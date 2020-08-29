#pragma once

#include <string>
#include "sigmaker-config.h"
#include "process-memory-editor.h"


class SigMaker {

private:
    ProcessMemoryEditor* mem;

public:
    SigMaker(ProcessMemoryEditor* mem);
    void appendSample(SigmakerConfig cfg);
    AobSig generateSignature(SigmakerConfig cfg);
    AobSig generateOptimalSignature(SigmakerConfig cfg);
};
