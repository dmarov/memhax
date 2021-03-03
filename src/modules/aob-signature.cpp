#include "aob-signature.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>

AOBSignature::AOBSignature() {}

AOBSignature::AOBSignature(const AOBSignature& sig) {
    this->len = sig.len;
    this->mask = sig.mask;
    std::memcpy(this->values, sig.values, this->len);
}

AOBSignature::AOBSignature(const std::byte* values, std::string mask)
{
    const auto len = mask.length();
    if (len > this->MAX_LEN)
    {
        throw std::exception("Failed to construct signature. Signature too large");
    }

    this->len = len;
    std::memcpy(this->values, values, this->len);
    this->mask = mask;
}


AOBSignature::AOBSignature(std::string signature) {

    std::regex reg("^(?:[0-9A-F]{2}|\\?{2})(?:\\s(?:[0-9A-F]{2}|\\?{2}))*$");

    if (!regex_match(signature, reg)) {
        throw std::exception("Failed to construct signature. Invalid argument.");
    }

    std::string::iterator end_pos = std::remove(signature.begin(), signature.end(), ' ');
    signature.erase(end_pos, signature.end());

    auto len = signature.length();
    auto sig_len = len / 2;

    if (sig_len > this->MAX_LEN)
    {
        throw new std::exception("Failed to construct signature. Signature too large");
    }

    this->len = sig_len;
    this->mask.resize(sig_len);

    std::stringstream ss;

    for (size_t i = 0; i < sig_len; i ++) {
        auto hex_str = signature.substr(2 * i, 2);
        if (hex_str.compare("??") != 0) {
            ss << std::hex << hex_str;
            unsigned hex_char;
            ss >> hex_char;
            ss.clear();
            this->values[i] = (std::byte)hex_char;
            this->mask[i] = 'x';
        } else {
            this->values[i] = (std::byte)'\x00';
            this->mask[i] = '?';
        }
    }
}

const std::byte* AOBSignature::getValues() const
{
    return this->values;
}

std::string AOBSignature::getMask() const
{
    return this->mask;
}

size_t AOBSignature::getLen() const
{
    return this->len;
}

std::ostream& operator<<(std::ostream &os, const AOBSignature& sig)
{
    std::stringstream svalues;

    svalues << std::hex << std::uppercase;
    auto values = sig.values;

    for (size_t i = 0; i < sig.len; ++i)
    {
        if (sig.mask[i] != '?') {
            svalues << std::setw(2) << std::setfill('0') << (unsigned)sig.values[i] << " ";
        } else {
            svalues << "?? ";
        }
    }

    os << svalues.str();

    return os;
}

/* AOBSignature AOBSignature::shrink(unsigned before, unsigned after) */
/* { */
/*     unsigned sig_start = 0, sig_end = 0; */

/*     for (int i = this->offset; i >= 0; --i) */
/*     { */
/*         if (mask[i] != '?') */
/*         { */
/*             --before; */
/*             sig_start = i; */
/*         } */

/*         if (before == 0) */
/*         { */
/*             break; */
/*         } */
/*     } */

/*     for (int i = this->offset; i < this->len; ++i) */
/*     { */
/*         if (mask[i] != '?') */
/*         { */
/*             --after; */
/*             sig_end = i; */
/*         } */

/*         if (after == 0) */
/*         { */
/*             break; */
/*         } */
/*     } */

/*     if (before != 0 || after != 0) */
/*     { */
/*         throw new std::exception("failed to shrink signature"); */
/*     } */

/*     unsigned sig_len = sig_end - sig_start; */
/*     unsigned sig_offset = this->offset - sig_start; */
/*     std::string sig_mask = this->mask.substr(sig_start, sig_len); */

/*     AOBSignature sig(this->values + sig_start, this->mask, sig_offset); */

/*     return sig; */
/* } */

AOBSignature::~AOBSignature()
{

}
