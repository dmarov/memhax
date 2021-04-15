#pragma once

#include "modules/process-memory-editor.h"

class AcHackLoop {

private:
    const memhax::ProcessMemoryEditor* editor;
    bool interupted;

public:
    AcHackLoop(const memhax::ProcessMemoryEditor& editor);
    void start();
};
