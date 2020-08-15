#include "process-memory-editor.h"

void ProcessMemoryEditor::setFloat(MultiLvlPtr ptr, float value) {

    this->writeAtMultiLvlPointer(ptr, (void*)&value, sizeof(value));
}
