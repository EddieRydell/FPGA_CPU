#pragma once

#include <windows.h>
#include <vector>
#include <cstdint>


class uart_transmitter {
public:
    uart_transmitter() = default;
    ~uart_transmitter();
    void send_bytes(const std::vector<uint8_t>& data) const;

private:
    void setup_serial(const char* com_port);
    HANDLE serial_handle;

};
