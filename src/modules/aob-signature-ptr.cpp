#include "aob-signature-ptr.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, size_t length)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->length = length;
}

const AOBSignature& AOBSignaturePtr::getSignature() const
{
    return *(this->signature);
}

uintptr_t AOBSignaturePtr::getBegin() const
{
    return this->begin;
}

size_t AOBSignaturePtr::getLenth() const
{
    return this->length;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}
