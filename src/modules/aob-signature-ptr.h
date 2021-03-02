#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"
#include "module-info.h"

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    uintptr_t begin;
    std::vector<ModuleInfo> scan_modules;

public:
    AOBSignaturePtr(std::string pattern, uintptr_t begin, std::vector<ModuleInfo> modules);

    const AOBSignature& getSignature() const;
    uintptr_t getBegin() const;
    const std::vector<ModuleInfo>& getScanModules() const;

    ~AOBSignaturePtr();
};
