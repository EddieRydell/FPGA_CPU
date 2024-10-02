#pragma once

#include <windows.h>
#include <vector>
#include <cstdint>
#include <string>


class uart_transmitter {
public:
    uart_transmitter() = default;
    ~uart_transmitter();
    void initialize();
    void send_bytes(const std::vector<uint8_t>& data) const;

private:
    bool probe_com_port(const std::string& com_port) const;
    static std::vector<std::string> list_com_ports();
    void setup_serial(const char* com_port);
    HANDLE serial_handle = nullptr;

};
