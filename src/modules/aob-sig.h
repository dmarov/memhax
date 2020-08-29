#pragma once

#include <string>
#include <vector>

class AobSig {

private:
    size_t len;
    std::byte* values;
    std::string mask;
    unsigned offset;

public:
    AobSig(const std::byte* values, std::string mask, unsigned offset);
    AobSig();
    AobSig(const AobSig& sig);
    const std::byte* getValues() const;
    std::string getMask() const;
    size_t getLength() const;
    unsigned getOffset() const;
    AobSig shrink(unsigned before, unsigned after);

    ~AobSig();

    friend std::ostream& operator<<(std::ostream &os, const AobSig& sig);
};
