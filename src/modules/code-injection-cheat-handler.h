#pragma once

#include "process-memory-editor.h"
#include "aob-signature-ptr.h"

class CodeInjectionCheatHandler {

private:
    const ProcessMemoryEditor* editor;
    uintptr_t regular_pointer;
    const static short max_len = 100;
    short length;

    char nops[max_len];
    char saved_value[max_len];
    bool initialized;
    bool enabled;

public:

    CodeInjectionCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, const std::byte* instructions, short length);

    void enable();
    void disable();

    ~CodeInjectionCheatHandler();
};
