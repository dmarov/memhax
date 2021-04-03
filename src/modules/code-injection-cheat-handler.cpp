#include "code-injection-cheat-handler.h"
#include "process-memory-editor.h"
#include "aob-signature-ptr.h"
#include <cstring>
#include <exception>
#include <iostream>

CodeInjectionCheatHandler::CodeInjectionCheatHandler(const ProcessMemoryEditor& editor, const AOBSignaturePtr& ptr, const std::vector<std::byte>& instructions)
{
    this->editor = &editor;
    this->instructions_size = instructions.size();
    this->alloc_size = this->instructions_size + 1 + this->editor->getPointerSize();
    this->instructions = new std::byte[this->instructions_size];

    for (size_t i = 0; i < this->instructions_size; ++i) {
        this->instructions[i] = instructions.at(i);
    }

    this->regular_pointer = this->editor->getRegularPointer(ptr);
    this->initialized = false;
    this->enabled = false;

    this->replace_size = 1 + this->editor->getPointerSize();
    this->new_jmp_instruction = new std::byte[this->replace_size];
    this->saved_value = new std::byte[this->replace_size];
    this->new_jmp_instruction[0] = (std::byte)0xFF;

    this->jmp_addr = this->editor->allocate(this->alloc_size);

    std::memcpy(this->new_jmp_instruction + 1, (void*)this->jmp_addr, this->editor->getPointerSize());
}

void CodeInjectionCheatHandler::enable()
{
    if (!this->enabled)
    {
        if (!this->initialized) {
            this->initialized = true;
            this->editor->read_p(this->regular_pointer, this->saved_value, this->replace_size);
        }

        this->editor->write_p(this->regular_pointer, this->new_jmp_instruction, this->replace_size);
        this->enabled = true;
    }
}

void CodeInjectionCheatHandler::disable()
{
    if (this->enabled)
    {
        this->editor->write_p(this->regular_pointer, this->saved_value, this->replace_size);
        this->enabled = false;
    }
}

CodeInjectionCheatHandler::~CodeInjectionCheatHandler()
{
    if (this->enabled)
    {
        this->disable();
    }

    this->editor->free(this->jmp_addr, this->instructions_size);

    delete []this->instructions;
    delete []this->new_jmp_instruction;
    delete []this->saved_value;
}
