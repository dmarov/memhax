#pragma once

#include <string>
#include <vector>

class CachedSignature {

private:
    char* values;
    char* mask;
    unsigned offset;
    unsigned size;

public:
    CachedSignature(char* values, char* mask, unsigned offset);
    char* getValues();
    char* getMask();
    unsigned getOffset();
};
