#include "sigmaker.h"
#include "sigmaker-config.h"
#include "win-api-process-memory-editor.h"
#include "multi-lvl-ptr.h"
#include "sigmaker-data-mapper.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>

SigMaker::SigMaker(ProcessMemoryEditor* mem)
{
    this->mem = mem;
}

void SigMaker::appendSample(SigmakerConfig cfg)
{
    auto [module_start_addr, module_len] = mem->getModuleInfo(cfg.getModuleName());

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

AobSig SigMaker::generateSignature(SigmakerConfig config)
{
    SigmakerDataMapper mapper;
    auto samples = mapper.selectSamples(config.getSessionId(), config.getLength());
    auto len = config.getLength();
    auto offset = config.getOffset();
    auto size = config.getSize();

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

AobSig SigMaker::generateOptimalSignature(SigmakerConfig cfg)
{
    auto sig = this->generateSignature(cfg);

    /* auto [values, mask, offset] = SigMaker::calculateSignature(config); */
    /* SignatureConfig sig_cfg(values, mask, offset, scan_start, scan_len); */

    return sig;
}
