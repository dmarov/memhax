#include "instruction-nop-cheat-handler.h"
#include "process-memory-editor.h"
#include "aob-signature-ptr.h"

InstructionNopCheatHandler::InstructionNopCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, size_t length)
{
    for (size_t i = 0; i < this->max_len; ++i)
    {
        this->nops[i] = 0x90;
    }
}

void InstructionNopCheatHandler::enable()
{

}

void InstructionNopCheatHandler::disable()
{

}
