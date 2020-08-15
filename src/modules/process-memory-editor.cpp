#include "process-memory-editor.h"

void ProcessMemoryEditor::setFloat(MultiLvlPtr ptr, float value) {

    this->writeAtMultiLvlPointer(ptr, (void*)&value, sizeof(value));
}

void ProcessMemoryEditor::readAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes)
{


}

void ProcessMemoryEditor::writeAtMultiLvlPointer(MultiLvlPtr, void* value, size_t n_bytes)
{


}
