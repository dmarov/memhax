#pragma once

#include <string>
#include <vector>

class CachedSignature {

private:
    std::string values;
    std::string mask;
    unsigned offset;
    unsigned size;

public:
    CachedSignature(std::string values, std::string mask, unsigned offset);
    std::string getValues();
    std::string getMask();
    unsigned getOffset();
};
