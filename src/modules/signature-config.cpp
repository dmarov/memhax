#include "signature-config.h"

SignatureConfig::SignatureConfig(std::string values, std::string mask, unsigned offset, uintptr_t scan_start_addr, size_t scan_len)
{
    this->values = values;
    this->mask = mask;
    this->offset = offset;
    this->scan_start_addr = scan_start_addr;
    this->scan_len = scan_len;
}

std::string SignatureConfig::getValues()
{
    return this->values;
}

std::string SignatureConfig::getMask()
{
    return this->mask;
}

unsigned SignatureConfig::getOffset()
{
    return this->offset;
}

uintptr_t SignatureConfig::getScanStartAddr()
{
    return this->scan_start_addr;
}

size_t SignatureConfig::getScanLen()
{
    return this->scan_len;
}

SignatureConfig SignatureConfig::operator+(long long scan_offset)
{
    auto values = this->getValues();
    auto mask = this->getMask();
    auto offset = this->getOffset();
    auto scan_len = this->getScanLen();
    auto scan_start = this->getScanStartAddr() + scan_offset;

    SignatureConfig res(values, mask, offset, scan_start, scan_len);

    return res;
}
