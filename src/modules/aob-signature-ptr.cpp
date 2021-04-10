#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <vector>
#include "aob-signature-ptr.h"
#include "memory-span.h"

namespace memhax {

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, int begin, MemorySpan scan_span)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_spans.push_back(scan_span);
}

AOBSignaturePtr::AOBSignaturePtr(std::string pattern, int begin, const std::vector<MemorySpan>& scan_spans)
{
    this->signature = new AOBSignature(pattern);
    this->begin = begin;
    this->scan_spans = scan_spans;
}

const AOBSignature& AOBSignaturePtr::getSignature() const
{
    return *(this->signature);
}

int AOBSignaturePtr::getBegin() const
{
    return this->begin;
}

const std::vector<MemorySpan>& AOBSignaturePtr::getScanSpans() const
{
    return this->scan_spans;
}

AOBSignaturePtr::~AOBSignaturePtr()
{
    delete this->signature;
}

}
