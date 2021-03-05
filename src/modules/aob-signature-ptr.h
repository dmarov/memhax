#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"
#include "memory-span.h"

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    uintptr_t begin;
    std::vector<MemorySpan> scan_spans;

public:

    AOBSignaturePtr(std::string pattern, uintptr_t begin, const std::vector<MemorySpan>& scan_spans);
    AOBSignaturePtr(std::string pattern, uintptr_t begin, MemorySpan scan_span);

    const AOBSignature& getSignature() const;
    uintptr_t getBegin() const;
    const std::vector<MemorySpan>& getScanSpans() const;

    ~AOBSignaturePtr();
};
