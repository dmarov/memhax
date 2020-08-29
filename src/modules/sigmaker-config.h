#pragma once

#include <string>
#include <vector>
#include <windows.h>

class SigmakerConfig {

private:
    std::string session_id;
    std::wstring executable_name;
    std::wstring module_name;
    std::vector<size_t> offsets;
    unsigned size;
    int offset;
    unsigned len;

public:
    SigmakerConfig(std::string path_to_file);
    SigmakerConfig();

    std::wstring getExecutableName();
    std::string getSessionId();
    std::wstring getModuleName();
    std::vector<DWORD_PTR> getOffsets();
    unsigned getSize();
    int getOffset();
    unsigned getLength();
};
