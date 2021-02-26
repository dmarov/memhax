#pragma once

#include <string>
#include <vector>

class AOBSignature {

private:
    const static size_t max_len = 100;
    std::byte values[max_len];
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
