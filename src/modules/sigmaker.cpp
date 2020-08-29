#include "sigmaker.h"
#include "sigmaker-config.h"
#include "win-api-process-memory-editor.h"
#include "multi-lvl-ptr.h"
#include "sigmaker-data-mapper.h"
#include <exception>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>

SigMaker::SigMaker(SigmakerConfig cfg, ProcessMemoryEditor* mem)
{
    this->mem = mem;
    this->cfg = cfg;
}

void SigMaker::appendSample()
{
    auto cfg = this->cfg;
    auto [module_start_addr, module_len] = this->mem->getModuleInfo(cfg.getModuleName());

    MultiLvlPtr mptr(module_start_addr, cfg.getOffsets());
    uintptr_t ptr = mem->getRegularPointer(mptr);
    ptr += cfg.getOffset();

    unsigned len = cfg.getLength();
    char* bytes = new char[len];

    mem->read(ptr, bytes, len);

    SigmakerDataMapper mapper;
    mapper.appendSample(cfg.getSessionId(), (std::byte*)bytes, len, cfg.getOffset(), cfg.getSize());

    delete[] bytes;
}

AobSig SigMaker::generateSignature()
{
    SigmakerDataMapper mapper;
    auto cfg = this->cfg;
    auto samples = mapper.selectSamples(cfg.getSessionId(), cfg.getLength());
    auto len = cfg.getLength();
    auto offset = cfg.getOffset();
    auto size = cfg.getSize();

    std::byte* result_bytes = new std::byte[len];

    for (unsigned i = 0; i < len; ++i)
    {
        result_bytes[i] = (std::byte)0x00;
    }

    std::vector<std::byte*>::iterator it = samples.begin();

    if (it == samples.end())
    {
        throw new std::exception("not enough samples to generate signature");
    }

    std::byte* ptr = *it;

    while (it != samples.end())
    {
        for (unsigned i = 0; i < len; ++i)
        {
            if (result_bytes[i] == (std::byte)0x00)
            {
                result_bytes[i] |= ptr[i] ^ (*it)[i];
            }
        }

        ++it;
    }

    std::stringstream result_values;
    std::stringstream result_mask;

    result_values << std::hex << std::uppercase;

    for (unsigned i = 0; i < len; ++i)
    {
        if (result_bytes[i] == (std::byte)0x00)
        {
            result_values << "\\0x" << std::setw(2) << std::setfill('0') << (unsigned)ptr[i];
            result_mask << "x";
        }
        else
        {
            result_values << "\\0x00";
            result_mask << "?";
        }
    }

    AobSig res(result_values.str(), result_mask.str(), -offset);

    return res;
}

AobSig SigMaker::generateOptimalSignature()
{
    auto cfg = this->cfg;
    auto sig = this->generateSignature();
    unsigned before = 2, after = 2;
    auto sig_buf = sig.shrink(before, after);
    std::wstring module_name = cfg.getModuleName();
    auto [module_start, module_size] = this->mem->getModuleInfo(module_name);

    AobSigCfg sig_cfg(sig_buf, module_start, module_size);

    unsigned cnt;
    while ((cnt = this->mem->getRegularPointers(sig_cfg, 2).size()) > 1)
    {
        ++before;
        ++after;
        sig_buf = sig.shrink(before, after);
        AobSigCfg sig_cfg(sig_buf, module_start, module_size);
    }

    if (cnt < 1)
    {
        throw new std::exception("failed to generate optimal signature");
    }

    return sig_buf;
}
