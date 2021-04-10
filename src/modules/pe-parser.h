#pragma once

#include <string>
#include "modules/dos-header.h"

namespace memhax {

class PEParser {

private:
    constexpr static std::byte MAGIC_BYTES[2] = {(std::byte)'M', (std::byte)'Z'};

private:
    std::byte* buffer;
    DOSHeader* header;


public:
    PEParser(const std::wstring path);

    ~PEParser();
};

}
