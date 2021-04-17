#pragma once

#include <string>

class GenericMethod {

public:
    virtual void inject(std::wstring target_name, std::wstring lib_file) = 0;
};
