#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <vector>
#include "aob-signature-ptr.h"
#include "memory-span.h"

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, MemorySpan scan_span)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_span = scan_span;
}

const AOBSignature& AOBSignaturePtr::getSignature() const
{
    return *(this->signature);
}

uintptr_t AOBSignaturePtr::getBegin() const
{
    return this->begin;
}

const MemorySpan AOBSignaturePtr::getScanSpan() const
{
    return this->scan_span;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}
