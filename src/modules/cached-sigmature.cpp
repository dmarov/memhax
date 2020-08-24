#include "cached-signature.h"

CachedSignature::CachedSignature(std::string values, std::string mask, unsigned offset)
{
    this->values = values;
    this->mask = mask;
    this->offset = offset;
}

std::string CachedSignature::getValues()
{
    return this->values;
}

std::string CachedSignature::getMask()
{
    return this->mask;
}

unsigned CachedSignature::getOffset()
{
    return this->offset;
}
