#pragma once

#include <string>
#include "structs/dos-header.h"

namespace memhax {

class PEParser {

private:
    constexpr static std::byte MAGIC_BYTES[2] = {(std::byte)'M', (std::byte)'Z'};
    constexpr static std::byte DOS_SIGNATURE[4] = {
        (std::byte)'P', (std::byte)'E',
        (std::byte)'\0', (std::byte)'\0'
    };

private:
    std::byte* buffer;
    DOSHeader* header;


public:
    PEParser(const std::wstring path);

    ~PEParser();
};

}
