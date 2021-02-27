#pragma once

#include "process-memory-editor.h"
#include "aob-signature-ptr.h"

class InstructionNopCheatHandler {

private:
    const static short max_len = 100;
    char nops[max_len];

public:

    InstructionNopCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, size_t length);

    void enable();
    void disable();
};
