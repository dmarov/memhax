#pragma once

#include <string>
#include <vector>

class Signature {

private:
    char* values;
    char* mask;
    unsigned offset;
    unsigned size;

public:
    Signature(char* values, char* mask, unsigned offset, unsigned size);
    char* getValues();
    char* getMask();
    unsigned getOffset();
    unsigned getSize();
};
