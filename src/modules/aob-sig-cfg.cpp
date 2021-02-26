/* #include "aob-sig-cfg.h" */

/* AobSigCfg::AobSigCfg(AobSig sig, uintptr_t scan_start_addr, size_t scan_len) */
/* { */
/*     this->sig = sig; */
/*     this->scan_start_addr = scan_start_addr; */
/*     this->scan_len = scan_len; */
/* } */

/* AobSig AobSigCfg::getSignature() */
/* { */
/*     return this->sig; */
/* } */

/* uintptr_t AobSigCfg::getScanStartAddr() */
/* { */
/*     return this->scan_start_addr; */
/* } */

/* size_t AobSigCfg::getScanLen() */
/* { */
/*     return this->scan_len; */
/* } */

/* AobSigCfg AobSigCfg::operator+(long long scan_offset) */
/* { */
/*     auto sig = this->sig; */
/*     auto scan_len = this->getScanLen(); */
/*     auto scan_start = this->getScanStartAddr() + scan_offset; */

/*     AobSigCfg res(sig, scan_start, scan_len); */

/*     return res; */
/* } */
