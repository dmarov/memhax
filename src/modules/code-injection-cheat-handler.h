#pragma once

#include "process-memory-editor.h"
#include "aob-signature-ptr.h"

class CodeInjectionCheatHandler {

private:
    const static short max_len = 100;

    const ProcessMemoryEditor* editor;
    uintptr_t regular_pointer;

    std::byte* new_jmp_instruction;
    std::byte* saved_value;
    std::byte* instructions;
    size_t instructions_size;

    bool initialized;
    bool enabled;

    uintptr_t jmp_addr;
    size_t alloc_size;
    size_t replace_size;

public:

    CodeInjectionCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, const std::vector<std::byte>& instructions);

    void enable();
    void disable();

    ~CodeInjectionCheatHandler();
};
