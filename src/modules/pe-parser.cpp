#include "pe-parser.h"
#include <string>
#include <fstream>
#include <iostream>

namespace memhax {

PEParser::PEParser(const std::wstring path)
{
    std::ifstream lib_file;

    lib_file.open(path, std::ios::binary);
    lib_file.seekg(0, std::ios::end);

    auto length = lib_file.tellg();

    lib_file.seekg(0, std::ios::beg);
    this->buffer = new std::byte[length];

    lib_file.read((char*)buffer, length);
    lib_file.close();

    for (auto i = 0; i < 30; ++i)
    {
        std::cout << "0x" << std::hex << (int)buffer[i] << " ";
    }

    std::cout << std::endl;

    if (buffer[0] != this->MAGIC_BYTES[0] || buffer[1] != this->MAGIC_BYTES[1])
    {
        throw std::exception("wrong PE file magic number");
    }

    this->header = (DOSHeader*)this->buffer;
}


PEParser::~PEParser()
{
    delete[] this->buffer;
}

}
