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
            break;
        }
    }
    std::cout << "UART initialization done\n";
}

void uart_transmitter::setup_serial(const char* com_port) {
    std::cout << "Setting up serial port on " << com_port << std::endl;
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
    dcbSerialParams.ByteSize = 8;             // Set ByteSize to 8 bits (1 byte)
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

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
    std::cout << "Successfully connected on port " << com_port << std::endl;
}

void uart_transmitter::send_bytes(HANDLE hSerial, const std::vector<uint8_t>& data) {
    DWORD bytes_written;
    std::cout << "Sending " << data.size() << " bytes..." << std::endl;

    for (size_t i = 0; i < data.size(); i += UART_CHUNK_SIZE) {
        BOOL success = WriteFile(hSerial, data.data() + i, UART_CHUNK_SIZE, &bytes_written, nullptr);
        if (!success || (bytes_written != UART_CHUNK_SIZE)) {
            throw std::runtime_error("Error writing byte to COM port");
        }
    }
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
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);
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
                std::string friendly_com_name = com_port_name.substr(com_pos);
                friendly_com_name.pop_back(); // remove the trailing parenthesis
                com_ports.push_back(friendly_com_name);
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

bool uart_transmitter::probe_com_port(const std::string& com_port) {
    std::string full_com_port = R"(\\.\)" + com_port; // "\\.\COMx" format for Windows
    std::cout << "Attempting to probe COM port: " << com_port << std::endl;

    setup_serial(com_port.c_str());

    const std::vector<uint8_t> message(1, 'P');
    std::cout << "Attempting to ping FPGA..." << std::endl;
    // Send out the message 'P', then wait for the response from the FPGA, which should be "A"
    try {
        send_bytes(serial_handle, message);

        // Wait and read the response from the FPGA
        char response;
        DWORD bytes_read;
        BOOL success = ReadFile(serial_handle, &response, 1, &bytes_read, nullptr);
        if (!success) {
            DWORD error = GetLastError();
            printf("ReadFile failed with error code: %ld\n", error);
        }
        if (success && bytes_read > 0) {
            std::cout << "Received response from FPGA: " << response << std::endl;
            if (response == 'A') {
                std::cout << "FPGA detected on " << com_port << std::endl;
                return true;
            }
        }
        std::cerr << "No valid response from FPGA on COM port: " << com_port << std::endl;

    }
    catch (std::runtime_error& e) {
        std::cerr << "Failed to find FPGA on COM port: " << com_port;
        std::cerr << ". Message: " << e.what() << std::endl;
        return false;
    }

    CloseHandle(serial_handle);
    return true;
}



