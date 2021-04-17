#pragma once

#include "generic-method.h"

class CreateRemoteThreadMethod : public GenericMethod {

public:
    void inject(std::wstring target_name, std::wstring lib_file);
};
