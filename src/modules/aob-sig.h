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
    std::string getValues() const;
    std::string getMask() const;
    unsigned getOffset() const;
    AobSig shrink(unsigned before, unsigned after);

    friend std::ostream& operator<<(std::ostream &os, const AobSig& sig);
};
