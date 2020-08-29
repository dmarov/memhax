#pragma once

#include <string>
#include <vector>

class AobSig {

private:
    std::string values;
    std::string mask;
    unsigned offset;

public:
    AobSig(std::string values, std::string mask, unsigned offset);
    AobSig();
    std::string getValues();
    std::string getMask();
    unsigned getOffset();
    AobSig shrink(unsigned before, unsigned after);

    std::ostream& operator<<(std::ostream &os);
};
