#include "aob-signature-ptr.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, uintptr_t scan_begin, size_t scan_length)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_begin = scan_begin;
    this->scan_length = scan_length;
}

const AOBSignature& AOBSignaturePtr::getSignature() const
{
    return *(this->signature);
}

uintptr_t AOBSignaturePtr::getBegin() const
{
    return this->begin;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}
