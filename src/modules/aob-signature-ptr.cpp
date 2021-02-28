#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <vector>
#include "aob-signature-ptr.h"
#include "module-info.h"

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, uintptr_t scan_begin, size_t scan_length)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_begin = scan_begin;
    this->scan_length = scan_length;
}

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, std::vector<ModuleInfo> modules)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_modules = modules;
}

const AOBSignature& AOBSignaturePtr::getSignature() const
{
    return *(this->signature);
}

uintptr_t AOBSignaturePtr::getBegin() const
{
    return this->begin;
}

uintptr_t AOBSignaturePtr::getScanBegin() const
{
    return this->scan_begin;
}

uintptr_t AOBSignaturePtr::getScanLength() const
{
    return this->scan_length;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}
