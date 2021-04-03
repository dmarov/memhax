#pragma once

#include "process-memory-editor.h"
#include "aob-signature-ptr.h"

class CodeInjectionHandler {

private:
    const static short max_len = 100;
    size_t ptr_size;

    const ProcessMemoryEditor* editor;
    uintptr_t regular_pointer;

    std::byte* new_jmp_instruction;
    std::byte* saved_value;
    std::byte* inj_instructions;
    size_t inj_instructions_size;

    bool initialized;
    bool enabled;

    uintptr_t jmp_addr;
    size_t alloc_size;
    size_t replace_size;
    size_t ptr_instruction_size;

public:

    CodeInjectionHandler(
        const ProcessMemoryEditor& editor,
        const AOBSignaturePtr& ptr,
        size_t ptr_instruction_size,
        const std::vector<std::byte>& instructions
    );

    void enable();
    void disable();

    ~CodeInjectionHandler();
};
