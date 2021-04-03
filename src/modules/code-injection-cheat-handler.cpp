#include "code-injection-cheat-handler.h"
#include "process-memory-editor.h"
#include "aob-signature-ptr.h"
#include <exception>
#include <iostream>

CodeInjectionCheatHandler::CodeInjectionCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, const std::byte* instructions, short length)
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
    this->regular_pointer = this->editor->getRegularPointer(ptr);
    this->initialized = false;
    this->enabled = false;
}

void CodeInjectionCheatHandler::enable()
{
    if (!this->enabled)
    {
        if (!this->initialized) {
            this->initialized = true;
            this->editor->read_p(this->regular_pointer, this->saved_value, this->length);
        }

        this->editor->write_p(this->regular_pointer, this->nops, this->length);
        this->enabled = true;
    }
}

void CodeInjectionCheatHandler::disable()
{
    if (this->enabled)
    {
        this->editor->write_p(this->regular_pointer, this->saved_value, this->length);
        this->enabled = false;
    }
}

CodeInjectionCheatHandler::~CodeInjectionCheatHandler()
{
    if (this->enabled)
    {
        this->disable();
    }
}
