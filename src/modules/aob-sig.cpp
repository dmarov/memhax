#include "aob-sig.h"
#include <iostream>
#include <sstream>
#include <iomanip>

AobSig::AobSig() {}

AobSig::AobSig(const std::byte* values, std::string mask, unsigned offset)
{
    this->len = mask.length();
    this->values = new std::byte[len];
    this->mask = mask;
    std::memcpy(this->values, values, len);
    this->offset = offset;
}

const std::byte* AobSig::getValues() const
{
    return this->values;
}

std::string AobSig::getMask() const
{
    return this->mask;
}

size_t AobSig::getLength()
{
    return this->len;
}

unsigned AobSig::getOffset() const
{
    return this->offset;
}

std::ostream& operator<<(std::ostream &os, const AobSig& sig)
{
    std::stringstream svalues;

    svalues << std::hex << std::uppercase;
    auto values = sig.getValues();

    for (unsigned i = 0; i < sig.len; ++i)
    {
        svalues << "\\0x" << std::setw(2) << std::setfill('0') << (unsigned)sig.values[i];
    }

    os << "{" <<
        "  \"values\": " << "\"" << svalues.str() << "\"" << std::endl <<
        "  \"mask\":   " << "\"" << sig.mask << "\"" << std::endl <<
        "  \"offset\": " << sig.offset << std::endl <<
        "}";

    return os;
}

AobSig AobSig::shrink(unsigned before, unsigned after)
{
    unsigned sig_start = 0, sig_end = 0;

    for (int i = this->offset; i >= 0; --i)
    {
        if (mask[i] != '?')
        {
            --before;
            sig_start = i;
        }

        if (before == 0)
        {
            break;
        }
    }

    for (int i = this->offset; i < this->len; ++i)
    {
        if (mask[i] != '?')
        {
            --after;
            sig_end = i;
        }

        if (after == 0)
        {
            break;
        }
    }

    if (before != 0 || after != 0)
    {
        throw new std::exception("failed to shrink signature");
    }

    unsigned sig_len = sig_end - sig_start;
    unsigned sig_offset = this->offset - sig_start;
    std::string sig_mask = this->mask.substr(sig_start, sig_len);

    AobSig sig(this->values + sig_start, this->mask, sig_offset);

    return sig;
}

AobSig::~AobSig()
{
    delete[] this->values;
}
