#pragma once

#include <string>

namespace memhax {

class PEParser {

private:
    const static std::byte MAGIC_1 = (std::byte)0x4D;
    const static std::byte MAGIC_2 = (std::byte)0x5A;

private:
    std::byte* buffer;

public:
    PEParser(const std::wstring path);

    ~PEParser();
};

}
