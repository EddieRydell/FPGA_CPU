#include "../headers/uart_transmitter.h"

#include <iostream>
#include <vector>
#include <cstdint>
#include <devguid.h>
#include <setupapi.h>

#define BAUD_RATE CBR_115200

uart_transmitter::uart_transmitter() {
    std::vector<std::string> com_ports = list_com_ports();
    for (const auto& i : com_ports) {
        if (probe_com_port(i)) {
            setup_serial(i.c_str());
            break;
        }
    }
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
    BOOL success = WriteFile(serial_handle, data.data(), data.size(), &bytes_written, NULL);

    if (!success) {
        throw std::runtime_error("Error writing to COM port");
    }
    else {
        std::cout << "Sent " << bytes_written << " bytes to the FPGA." << std::endl;
    }
}

uart_transmitter::~uart_transmitter() {
    if (serial_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(serial_handle);
    }
}

// Function to list all available COM ports
std::vector<std::string> uart_transmitter::list_com_ports() {
    std::vector<std::string> com_ports;

    // Get the list of all devices in the Ports (COM & LPT) class
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        return com_ports; // Return empty if we can't get device information
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

    SetupDiDestroyDeviceInfoList(hDevInfo); // Cleanup
    return com_ports;
}

bool uart_transmitter::probe_com_port(const std::string& com_port) const {
    std::string full_com_port = R"(\\.\)" + com_port; // "\\.\COMx" format for Windows

    HANDLE hSerial = CreateFile(full_com_port.c_str(),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                nullptr,
                                OPEN_EXISTING,
                                0,
                                nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        // std::cerr << "Failed to open " << com_port << std::endl;
        return false;
    }

    const std::string ping_message = "PING";
    const std::vector<uint8_t> message(ping_message.begin(), ping_message.end());
    std::cout << "Attempting to ping FPGA\n";
    try {
        send_bytes(message);
    }
    catch (std::runtime_error&) {
        std::cerr << "Failed to find FPGA on COM port: " << com_port;
        return false;
    }



    CloseHandle(hSerial);
    return true;
}

