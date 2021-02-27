#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    uintptr_t begin;
    size_t length;

public:
    AOBSignaturePtr(std::string pattern, uintptr_t begin, size_t length);

    const AOBSignature& getSignature() const;
    uintptr_t getBegin() const;
    size_t getLenth() const;

    ~AOBSignaturePtr();
};
