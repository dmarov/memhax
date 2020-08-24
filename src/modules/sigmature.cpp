#include "signature.h"

Signature::Signature(char* values, char* mask, unsigned offset, unsigned size)
{
    this->values = values;
    this->mask = mask;
    this->offset = offset;
    this->size = size;
}

char* Signature::getValues()
{
    return this->values;
}

char* Signature::getMask()
{
    return this->mask;
}

unsigned Signature::getOffset()
{
    return this->offset;
}

unsigned Signature::getSize()
{
    return this->size;
}
