#include "cached-signature.h"

CachedSignature::CachedSignature(char* values, char* mask, unsigned offset)
{
    this->values = values;
    this->mask = mask;
    this->offset = offset;
}

char* CachedSignature::getValues()
{
    return this->values;
}

char* CachedSignature::getMask()
{
    return this->mask;
}

unsigned CachedSignature::getOffset()
{
    return this->offset;
}
