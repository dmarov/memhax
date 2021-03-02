#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <vector>
#include "aob-signature-ptr.h"
#include "module-info.h"

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, const std::vector<std::wstring>& scan_module_names)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_module_names = scan_module_names;
}

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin, std::wstring module_name)
    : AOBSignaturePtr(pattern, begin, std::vector {module_name})
{ }

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, uintptr_t begin)
    : AOBSignaturePtr(pattern, begin, std::vector<std::wstring> {})
{ }

const AOBSignature& AOBSignaturePtr::getSignature() const
{
    return *(this->signature);
}

uintptr_t AOBSignaturePtr::getBegin() const
{
    return this->begin;
}

const std::vector<std::wstring>& AOBSignaturePtr::getScanModuleNames() const
{
    return this->scan_module_names;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}
