#ifndef __SIGMAKER_H__
#define __SIGMAKER_H__

#include <string>
#include "sigmaker-config.h"

class SigMaker {

public:
    // appends record in database for specified file
    static void appendSample(std::string path_to_config);

    // generates signature based on database data
    static std::tuple<std::string, std::string, unsigned> generateSignature(SigmakerConfig config);

    static std::tuple<std::string, std::string, unsigned> calculateSignature(SigmakerConfig config);

    // deletes all records related to config
    static int resetSignature(std::string path_to_config);

private:
};

#endif
