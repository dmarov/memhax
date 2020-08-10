#ifndef __SIGMAKER_H__
#define __SIGMAKER_H__

#include <string>

class SigMaker {

public:
    // appends record in database for specified file
    static void appendSample(std::string path_to_config);

    // generates signature based on database data
    static std::string generateSignature(std::string path_to_config);

    // deletes all records related to config
    static int resetSignature(std::string path_to_config);

private:
};

#endif
