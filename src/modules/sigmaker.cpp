#include "sigmaker.h"
#include "sigmaker-config.h"
#include "win-api-process-memory-editor.h"
#include "multi-lvl-ptr.h"
#include "sigmaker-data-mapper.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>

void SigMaker::appendSample(std::string path_to_config)
{
    SigmakerConfig config(path_to_config);
    WinApiProcessMemoryEditor mem(config.getExecutableName());

    auto [module_start_addr, module_len] = mem.getModuleInfo(config.getModuleName());

    MultiLvlPtr mptr(module_start_addr, config.getOffsets());
    uintptr_t ptr = mem.getRegularPointer(mptr);
    ptr += config.getOffset();

    unsigned len = config.getLength();
    char* bytes = new char[len];

    mem.read(ptr, bytes, len);

    SigmakerDataMapper mapper;
    mapper.appendSample(config.getSessionId(), (std::byte*)bytes, len, config.getOffset(), config.getSize());

    delete[] bytes;
}

std::tuple<std::string, std::string, unsigned> SigMaker::calculateSignature(SigmakerConfig config)
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

    return std::tuple<std::string, std::string, unsigned>(result_values.str(), result_mask.str(), -offset);
}

std::tuple<std::string, std::string, unsigned> SigMaker::generateSignature(SigmakerConfig config)
{
    auto [values, mask, offset] = SigMaker::calculateSignature(config);
    SignatureConfig sig_cfg(values, mask, offset, scan_start, scan_len);

}

int SigMaker::resetSignature(std::string path_to_config)
{
    return 0;
}

