#pragma once

#include <string>
#include <vector>

class SignatureConfig {

private:
    std::string values;
    std::string mask;
    unsigned offset;
    uintptr_t scan_start_addr;
    size_t scan_len;

public:
    SignatureConfig(std::string values, std::string mask, unsigned offset, uintptr_t scan_start_addr, size_t scan_len);
    std::string getValues();
    std::string getMask();
    unsigned getOffset();
    uintptr_t getScanStartAddr();
    size_t getScanLen();

    SignatureConfig operator+(const long long int offset);
};
