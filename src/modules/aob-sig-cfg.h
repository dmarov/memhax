#pragma once

#include <string>
#include <vector>
#include "aob-sig.h"

class AobSigCfg {

private:
    AobSig sig;
    uintptr_t scan_start_addr;
    size_t scan_len;

public:
    AobSigCfg(std::string values, std::string mask, unsigned offset, uintptr_t scan_start_addr, size_t scan_len);
    AobSigCfg(AobSig sig, uintptr_t scan_start_addr, size_t scan_len);

    AobSig getSignature();
    uintptr_t getScanStartAddr();
    size_t getScanLen();

    AobSigCfg operator+(const long long int offset);
};