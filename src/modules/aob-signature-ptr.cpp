#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <vector>
#include "aob-signature-ptr.h"
#include "module-info.h"

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

const std::vector<ModuleInfo>& AOBSignaturePtr::getScanModules() const
{
    return this->scan_modules;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}
