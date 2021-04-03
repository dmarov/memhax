#pragma once

#include "process-memory-editor.h"
#include "aob-signature-ptr.h"

class InstructionNopHandler {

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

    InstructionNopHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, short length);

    void enable();
    void disable();

    ~InstructionNopHandler();
};
