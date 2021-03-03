#pragma once

#include <string>
#include <vector>

class AOBSignature {

public:
    const static size_t MAX_LEN = 100;

private:
    std::byte values[MAX_LEN];
    std::string mask;
    unsigned len;

public:
    AOBSignature(const std::byte* values, std::string mask);
    AOBSignature(std::string signature);
    AOBSignature();
    AOBSignature(const AOBSignature& sig);

    const std::byte* getValues() const;
    std::string getMask() const;
    size_t getLen() const;

    ~AOBSignature();

    friend std::ostream& operator<<(std::ostream &os, const AOBSignature& sig);
};
