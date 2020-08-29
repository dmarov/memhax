#include "aob-sig.h"
#include <iostream>

AobSig::AobSig() {}

AobSig::AobSig(std::string values, std::string mask, unsigned offset)
{
    if (values.length() != mask.length())
    {
        throw new std::exception("failed to construct signature");
    }

    this->values = values;
    this->mask = mask;
    this->offset = offset;
}

std::string AobSig::getValues() const
{
    return this->values;
}

std::string AobSig::getMask() const
{
    return this->mask;
}

unsigned AobSig::getOffset() const
{
    return this->offset;
}

std::ostream& operator<<(std::ostream &os, const AobSig& sig)
{
    os << "{" <<
        "  \"values\": " << "\"" << sig.getValues() << "\"" << std::endl <<
        "  \"mask\": "<< sig.getMask() << "\"" << std::endl <<
        "  \"offset\": " << sig.getOffset() << "\"" << std::endl <<
        "}";

    return os;
}

AobSig AobSig::shrink(unsigned before, unsigned after)
{
    unsigned sig_start = 0, sig_end = 0;
    unsigned len = this->mask.length();

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

    for (int i = this->offset; i < len; ++i)
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

    std::string sig_mask = this->mask.substr(offset, sig_len);
    std::string sig_values = this->values.substr(offset, sig_len);
    unsigned sig_offset = this->offset - sig_start;

    AobSig sig(sig_values, sig_mask, sig_offset);

    return sig;
}
