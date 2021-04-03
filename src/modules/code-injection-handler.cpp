#include "code-injection-handler.h"
#include "process-memory-editor.h"
#include "aob-signature-ptr.h"
#include <cstring>
#include <exception>
#include <iostream>

CodeInjectionHandler::CodeInjectionHandler(
    const ProcessMemoryEditor& editor,
    const AOBSignaturePtr& ptr,
    size_t ptr_instruction_size,
    const std::vector<std::byte>& instructions
)
{
    this->initialized = false;
    this->enabled = false;
    this->editor = &editor;
    this->ptr_size = this->editor->getPointerSize();

    this->ptr_instruction_size = ptr_instruction_size;
    this->inj_instructions_size = instructions.size();
    this->alloc_size = this->inj_instructions_size + 1 + this->ptr_size;
    this->inj_instructions = new std::byte[this->inj_instructions_size];

    for (size_t i = 0; i < this->inj_instructions_size; ++i)
    {
        this->inj_instructions[i] = instructions.at(i);
    }

    this->inj_instructions[this->inj_instructions_size] = (std::byte)0xFF;

    this->regular_pointer = this->editor->getRegularPointer(ptr);
    std::memcpy(
        this->inj_instructions + this->inj_instructions_size + 1,
        (void*)this->regular_pointer,
        this->editor->getPointerSize()
    );

    this->replace_size = 1 + this->ptr_size;
    this->new_jmp_instruction = new std::byte[this->replace_size];
    this->saved_value = new std::byte[this->replace_size];
    this->new_jmp_instruction[0] = (std::byte)0xFF;

    this->jmp_addr = this->editor->allocate(this->alloc_size);
    this->editor->write_p(this->jmp_addr, this->inj_instructions, this->inj_instructions_size);

    std::memcpy(this->new_jmp_instruction + 1, (void*)this->jmp_addr, this->ptr_size);
}

void CodeInjectionHandler::enable()
{
    if (!this->enabled)
    {
        if (!this->initialized)
        {
            this->initialized = true;
            this->editor->read_p(this->regular_pointer, this->saved_value, this->replace_size);
        }

        this->editor->write_p(this->regular_pointer, this->new_jmp_instruction, this->replace_size);
        this->enabled = true;
    }
}

void CodeInjectionHandler::disable()
{
    if (this->enabled)
    {
        this->editor->write_p(this->regular_pointer, this->saved_value, this->replace_size);
        this->enabled = false;
    }
}

CodeInjectionHandler::~CodeInjectionHandler()
{
    if (this->enabled)
    {
        this->disable();
    }

    this->editor->free(this->jmp_addr, this->inj_instructions_size);

    delete []this->inj_instructions;
    delete []this->new_jmp_instruction;
    delete []this->saved_value;
}
