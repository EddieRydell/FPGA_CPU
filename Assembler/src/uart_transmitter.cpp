#include "../headers/uart_transmitter.h"

#include <iostream>
#include <vector>
#include <cstdint>
#include <devguid.h>
#include <setupapi.h>

#define BAUD_RATE CBR_115200
#define UART_CHUNK_SIZE 6       // 48 bit instruction size

void uart_transmitter::initialize() {
    std::cout << "Initializing UART transmitter...\n";
    std::vector<std::string> com_ports = list_com_ports();
    for (const auto& i : com_ports) {
        if (probe_com_port(i)) {
            setup_serial(i.c_str());
            break;
        }
    }
    std::cout << "UART initialization done\n";
}

void uart_transmitter::setup_serial(const char* com_port) {
    // Open the serial port
    HANDLE hSerial = CreateFile(com_port,
                                GENERIC_READ | GENERIC_WRITE,
                                0,    // No sharing
                                nullptr, // No security
                                OPEN_EXISTING,
                                0,    // Non-overlapped I/O
                                nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Error: Unable to open COM port!");
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error getting state of serial port");
    }

    dcbSerialParams.BaudRate = BAUD_RATE;
    dcbSerialParams.ByteSize = 8;             // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT;    // 1 stop bit
    dcbSerialParams.Parity = NOPARITY;        // No parity

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error setting serial port state");
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;         // Maximum time between bytes
    timeouts.ReadTotalTimeoutConstant = 50;    // Total read timeout
    timeouts.ReadTotalTimeoutMultiplier = 10;  // Multiplier per byte
    timeouts.WriteTotalTimeoutConstant = 50;   // Total write timeout
    timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplier per byte

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        CloseHandle(hSerial);
        throw std::runtime_error("Error setting timeouts");
    }
    serial_handle = hSerial;
}

void uart_transmitter::send_bytes(const std::vector<uint8_t>& data) const {
    DWORD bytes_written;

    if (data.size() % UART_CHUNK_SIZE != 0) {
        throw std::runtime_error("Data size is not a multiple of 6 bytes!");
    }

    for (size_t i = 0; i < data.size(); i += UART_CHUNK_SIZE) {
        BOOL success = WriteFile(serial_handle, data.data() + i, UART_CHUNK_SIZE, &bytes_written, nullptr);

        if (!success || (bytes_written != UART_CHUNK_SIZE)) {
            throw std::runtime_error("Error writing instruction to COM port");
        }
    }

    std::cout << "All instructions sent." << std::endl;
}

uart_transmitter::~uart_transmitter() {
    if (serial_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(serial_handle);
    }
}

// Function to list all available COM ports
std::vector<std::string> uart_transmitter::list_com_ports() {
    std::cout << "Listing all COM ports...\n";
    std::vector<std::string> com_ports;

    // Get the list of all devices in the Ports (COM & LPT) class
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_ALLCLASSES);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        std::cerr << "ERROR: Unable to find device info for COM and LPT ports\n";
        return com_ports;
    }
    SP_DEVINFO_DATA DeviceInfoData;
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // Iterate over all devices in the Ports class
    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++) {
        DWORD dataType;
        char buffer[256];
        DWORD bufferSize = 0;

        // Get the registry property SPDRP_FRIENDLYNAME for each device
        if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, &dataType,
                                             (BYTE*)buffer, sizeof(buffer), &bufferSize)) {
            std::string com_port_name(buffer);
            // Find and extract the COM port number from the friendly name
            size_t com_pos = com_port_name.find("COM");
            if (com_pos != std::string::npos) {
                com_ports.push_back(com_port_name.substr(com_pos));
            }
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    std::cout << "Found ports:\n";
    for (const auto& i : com_ports) {
        std::cout << i << std::endl;
    }
    return com_ports;
}

bool uart_transmitter::probe_com_port(const std::string& com_port) const {
    std::string full_com_port = R"(\\.\)" + com_port; // "\\.\COMx" format for Windows
    std::cout << "Attempting to probe COM port: " << com_port << std::endl;

    HANDLE hSerial = CreateFile(full_com_port.c_str(),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                nullptr,
                                OPEN_EXISTING,
                                0,
                                nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        std::cerr << "Failed to open " << com_port << ". Error Code: " << error << std::endl;
        return false;
    }
    std::cout << "Successfully connected on port" << com_port << std::endl;

    const std::string ping_message = "PING";
    const std::vector<uint8_t> message(ping_message.begin(), ping_message.end());
    std::cout << "Attempting to ping FPGA\n";
    // Send out the message "PING", then wait for the response from the FPGA, which should be "ACK"
    try {
        send_bytes(message);

        // Wait and read the response from the FPGA
        char response[4];
        DWORD bytes_read;
        BOOL success = ReadFile(hSerial, response, sizeof(response) - 1, &bytes_read, nullptr);

        if (success && bytes_read > 0) {
            response[bytes_read] = '\0';

            if (strcmp(response, "ACK") == 0) {
                std::cout << "FPGA detected on " << com_port << std::endl;
                CloseHandle(hSerial);
                return true;
            }
        }
        std::cerr << "No valid response from FPGA on COM port: " << com_port << std::endl;

    }
    catch (std::runtime_error&) {
        std::cerr << "Failed to find FPGA on COM port: " << com_port;
        return false;
    }

    CloseHandle(hSerial);
    return true;
}



