#pragma once

#include <string>
#include <vector>
#include "aob-signature.h"
#include "memory-span.h"

namespace memhax {

class AOBSignaturePtr {

private:
    const AOBSignature* signature;
    int begin;
    std::vector<MemorySpan> scan_spans;

public:

    AOBSignaturePtr(std::string pattern, int begin, const std::vector<MemorySpan>& scan_spans);
    AOBSignaturePtr(std::string pattern, int begin, MemorySpan scan_span);

    const AOBSignature& getSignature() const;
    int getBegin() const;
    const std::vector<MemorySpan>& getScanSpans() const;

    ~AOBSignaturePtr();
};

}
