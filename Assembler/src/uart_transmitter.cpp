#include "../headers/uart_transmitter.h"

#include <iostream>
#include <vector>
#include <cstdint>

#define BAUD_RATE CBR_115200

void uart_transmitter::setup_serial(const char* com_port) {
    // Open the serial port
    HANDLE hSerial = CreateFile(com_port,
                                GENERIC_READ | GENERIC_WRITE,
                                0,    // No sharing
                                NULL, // No security
                                OPEN_EXISTING,
                                0,    // Non-overlapped I/O
                                NULL);

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
    } else {
        std::cout << "Sent " << bytes_written << " bytes to the FPGA." << std::endl;
    }
}

uart_transmitter::~uart_transmitter() {
    if (serial_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(serial_handle);
    }
}
