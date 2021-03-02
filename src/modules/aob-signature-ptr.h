#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"
#include "module-info.h"

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    uintptr_t begin;
    std::vector<std::wstring> scan_module_names;

public:

    // TODO: create container for search regions
    AOBSignaturePtr(std::string pattern, uintptr_t begin);
    AOBSignaturePtr(std::string pattern, uintptr_t begin, std::wstring module);
    AOBSignaturePtr(std::string pattern, uintptr_t begin, const std::vector<std::wstring>& scan_module_names);

    const AOBSignature& getSignature() const;
    uintptr_t getBegin() const;
    const std::vector<std::wstring>& getScanModuleNames() const;

    ~AOBSignaturePtr();
};
