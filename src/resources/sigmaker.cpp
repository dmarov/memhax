/* #include "sigmaker.h" */
/* #include "sigmaker-config.h" */
/* #include "win-api-process-memory-editor.h" */
/* #include "multi-lvl-ptr.h" */
/* #include "sigmaker-data-mapper.h" */
/* #include <exception> */
/* #include <sstream> */
/* #include <iomanip> */
/* #include <iostream> */
/* #include <boost/algorithm/string.hpp> */

/* SigMaker::SigMaker(SigmakerConfig cfg, ProcessMemoryEditor* mem) */
/* { */
/*     this->mem = mem; */
/*     this->cfg = cfg; */
/* } */

/* void SigMaker::appendSample() */
/* { */
/*     auto cfg = this->cfg; */
/*     auto [module_start_addr, module_len] = this->mem->getModuleInfo(cfg.getModuleName()); */

/*     MultiLvlPtr mptr(module_start_addr, cfg.getOffsets()); */
/*     uintptr_t ptr = mem->getRegularPointer(mptr); */
/*     ptr += cfg.getOffset(); */

/*     unsigned len = cfg.getLength(); */
/*     char* bytes = new char[len]; */

/*     mem->read(ptr, bytes, len); */

/*     SigmakerDataMapper mapper; */
/*     mapper.appendSample(cfg.getSessionId(), (std::byte*)bytes, len, cfg.getOffset(), cfg.getSize()); */

/*     delete[] bytes; */
/* } */

/* AobSig SigMaker::generateSignature() */
/* { */
/*     auto cfg = this->cfg; */
/*     SigmakerDataMapper mapper; */
/*     auto samples = mapper.selectSamples(cfg.getSessionId(), cfg.getLength()); */

/*     auto len = cfg.getLength(); */
/*     std::byte* result_bytes = new std::byte[len]; */

/*     for (unsigned i = 0; i < len; ++i) */
/*     { */
/*         result_bytes[i] = (std::byte)0x00; */
/*     } */

/*     std::vector<std::byte*>::iterator it = samples.begin(); */

/*     if (it == samples.end()) */
/*     { */
/*         throw new std::exception("not enough samples to generate signature"); */
/*     } */

/*     std::byte* ptr = *it; */

/*     while (it != samples.end()) */
/*     { */
/*         for (unsigned i = 0; i < len; ++i) */
/*         { */
/*             if (result_bytes[i] == (std::byte)0x00) */
/*             { */
/*                 result_bytes[i] |= ptr[i] ^ (*it)[i]; */
/*             } */
/*         } */

/*         ++it; */
/*     } */

/*     std::stringstream result_mask; */

/*     for (unsigned i = 0; i < len; ++i) */
/*     { */
/*         if (result_bytes[i] == (std::byte)0x00) */
/*         { */
/*             result_bytes[i] = ptr[i]; */
/*             result_mask << 'x'; */
/*         } */
/*         else */
/*         { */
/*             result_bytes[i] = (std::byte)0x00; */
/*             result_mask << '?'; */
/*         } */
/*     } */

/*     auto offset = cfg.getOffset(); */

/*     AobSig res(result_bytes, result_mask.str(), -offset); */

/*     delete[] result_bytes; */

/*     for (auto sample : samples) */
/*     { */
/*         delete[] sample; */
/*     } */

/*     return res; */
/* } */

/* AobSig SigMaker::generateOptimalSignature() */
/* { */
/*     auto cfg = this->cfg; */
/*     auto sig = this->generateSignature(); */
/*     /1* unsigned before = 2, after = 2; *1/ */
/*     /1* auto sig_buf = sig.shrink(before, after); *1/ */
/*     /1* std::wstring module_name = cfg.getModuleName(); *1/ */
/*     /1* auto [module_start, module_size] = this->mem->getModuleInfo(module_name); *1/ */

/*     /1* AobSigCfg sig_cfg(sig_buf, module_start, module_size); *1/ */

/*     /1* unsigned cnt; *1/ */
/*     /1* while ((cnt = this->mem->getRegularPointers(sig_cfg, 2).size()) > 1) *1/ */
/*     /1* { *1/ */
/*     /1*     ++before; *1/ */
/*     /1*     ++after; *1/ */
/*     /1*     sig_buf = sig.shrink(before, after); *1/ */
/*     /1*     AobSigCfg sig_cfg(sig_buf, module_start, module_size); *1/ */
/*     /1* } *1/ */

/*     /1* if (cnt < 1) *1/ */
/*     /1* { *1/ */
/*     /1*     throw new std::exception("failed to generate optimal signature"); *1/ */
/*     /1* } *1/ */

/*     /1* return sig_buf; *1/ */
/*     return sig; */
/* } */
