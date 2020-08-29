#include "aob-sig.h"
#include <iostream>

AobSig::AobSig(std::string values, std::string mask, unsigned offset)
{
    this->values = values;
    this->mask = mask;
    this->offset = offset;
}

std::string AobSig::getValues()
{
    return this->values;
}

std::string AobSig::getMask()
{
    return this->mask;
}

unsigned AobSig::getOffset()
{
    return this->offset;
}

std::ostream& AobSig::operator<<(std::ostream &os)
{
    os << "{" <<
        "  \"values\": " << "\"" << this->values << "\"" << std::endl <<
        "  \"mask\": "<< this->mask << "\"" << std::endl <<
        "  \"offset\": " << this->offset << "\"" << std::endl <<
        "}";

    return os;
}
