#include "instruction-nop-cheat-handler.h"
#include "process-memory-editor.h"
#include "aob-signature-ptr.h"
#include <exception>

InstructionNopCheatHandler::InstructionNopCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, short length)
{
    if (length > this->max_len)
    {
        throw std::exception("signature is too large");
    }

    for (size_t i = 0; i < this->max_len; ++i)
    {
        this->nops[i] = 0x90;
    }

    this->editor = &editor;
    this->length = length;
    this->ptr = &ptr;
    this->initialized = false;
}

void InstructionNopCheatHandler::enable()
{
    if (!this->initialized) {
        this->initialized = true;
        this->editor->read(*(this->ptr), this->saved_value, this->length);
    }

    this->editor->write(*(this->ptr), this->nops, this->length);
}

void InstructionNopCheatHandler::disable()
{
    this->editor->write(*(this->ptr), this->saved_value, this->length);
}

InstructionNopCheatHandler::~InstructionNopCheatHandler()
{

}
