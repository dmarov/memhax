#ifndef __TRAINER_UTILS_H__
#define __TRAINER_UTILS_H__

#include <string>
#include <vector>
#include <windows.h>

class TrainerUtils {

public:
    TrainerUtils(std::string window_name);
    TrainerUtils(HANDLE prcess_handle);

    void readMpFloat(std::string module_name, std::vector<void*> offsets, float *result);
    void writeMpFloat(std::string module_name, std::vector<void*> offsets, float result);

    void findMemoryAddress(char* value, char* mask, size_t len);

};

#endif
