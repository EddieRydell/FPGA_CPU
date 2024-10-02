#pragma once

#include <string>
#include <vector>
#include <cstdint>

class instruction {
public:
    instruction() = default;
    instruction(const std::string&);

private:
    std::string text;
    static std::pair<const char, const uint32_t> string_to_arg(const std::string& arg);
    static std::vector<std::string> split(const std::string&);

protected:
    uint8_t op_code = 0;
    uint8_t fun_code = 0;
    uint8_t arg1 = 0;
    uint8_t arg2 = 0;
    uint8_t immediate[4] = {0};

    friend class program;
};

