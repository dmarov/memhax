#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    uintptr_t begin;
    uintptr_t scan_begin;
    size_t scan_length;

public:
    AOBSignaturePtr(std::string pattern, uintptr_t begin, uintptr_t scan_begin, size_t scan_length);

    const AOBSignature& getSignature() const;
    uintptr_t getBegin() const;

    ~AOBSignaturePtr();
};
