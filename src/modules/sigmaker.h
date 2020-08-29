#pragma once

#include <string>
#include "sigmaker-config.h"
#include "process-memory-editor.h"


class SigMaker {

private:
    ProcessMemoryEditor* mem;
    SigmakerConfig cfg;

public:
    SigMaker(SigmakerConfig cfg, ProcessMemoryEditor* mem);
    void appendSample();
    AobSig generateSignature();
    AobSig generateOptimalSignature();
};
