#pragma once

#include <string>

class GenericMethod {

public:
    virtual void inject(std::wstring process_name, std::wstring full_lib_path) = 0;
};
