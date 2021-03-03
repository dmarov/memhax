#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"
#include "memory-span.h"

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    uintptr_t begin;
    MemorySpan scan_span;

public:

    AOBSignaturePtr(std::string pattern, uintptr_t begin, std::tuple<uintptr_t, size_t>);

    const AOBSignature& getSignature() const;
    uintptr_t getBegin() const;
    const MemorySpan getScanSpan() const;

    ~AOBSignaturePtr();
};
