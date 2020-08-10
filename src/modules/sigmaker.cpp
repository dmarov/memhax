#include "sigmaker.h"
#include "config.h"
#include "scanner.h"
#include "data-mapper.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string.hpp>

void SigMaker::appendSample(std::string path_to_config)
{
    Config config(path_to_config);
    Scanner scanner(config.getWindowName());

    auto len = config.getLength();
    auto offset = config.getOffset();

    std::byte* bytes = scanner.readMemory(
        config.getModuleName(),
        config.getOffsets(),
        offset,
        len
    );

    DataMapper mapper;
    mapper.appendSample(config.getSessionId(), bytes, len, offset, config.getSize());
}

std::string SigMaker::generateSignature(std::string path_to_config)
{
    Config config(path_to_config);

    DataMapper mapper;
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
        std::string result;

        for (unsigned i = 0; i < len; ++i)
        {
            if (i >= -offset && i < -offset + size)
            {
                result += "?? ";
            }
            else
            {
                result += "?? ";
            }
        }

        boost::trim_right(result);

        return result;
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


    std::stringstream ss;
    ss << std::hex << std::uppercase;

    for (unsigned i = 0; i < len; ++i)
    {
        if (result_bytes[i] == (std::byte)0x00)
        {
            ss << std::setw(2) << std::setfill('0') << (unsigned)ptr[i] << " ";
        }
        else
        {
            if (i >= -offset && i < -offset + size)
            {
                ss << "?? ";
            }
            else
            {
                ss << "?? ";
            }
        }
    }

    std::string result = ss.str();
    boost::trim_right(result);

    return result;
}

int SigMaker::resetSignature(std::string path_to_config)
{
    return 0;
}

