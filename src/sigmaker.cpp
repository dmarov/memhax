#include <locale>
#include <sqlite3.h>
#include "modules/sigmaker.h"
#include "memhax.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <chrono>
#include <thread>
#include <windows.h>

namespace po = boost::program_options;


int main(int argc, char **argv)
{
    bool is_monitoring = false;

    try {

        po::options_description desc("Supported options");

        desc.add_options()
            ("help", "produce help message")
            ("version,v", "print version")
            ("config", po::value<std::string>(), "specify config")
            ("command", po::value<std::string>(), "specify command");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("version")) {
            std::cout << "v" << memhax_VERSION_MAJOR << "." << memhax_VERSION_MINOR << "." << memhax_VERSION_PATCH << std::endl;
            return 0;
        }

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 0;
        }

        if (!vm.count("config")) {
            std::cout << "specify config via --config option" << std::endl;
            return 1;
        }

        auto config = vm["config"].as<std::string>();

        if (!vm.count("command"))
        {
            std::cout << "specify command via --command option" << std::endl;
            return 1;
        }

        auto command = vm["command"].as<std::string>();

        if (command.compare("append") == 0)
        {
            SigMaker::appendSample(config);
            std::cout << "appended new sample" << std::endl;
            return 0;
        }

        if (command.compare("generate") == 0)
        {
            auto [values, mask, offset] = SigMaker::generateSignature(config);

            std::cout << "values: " << values << std::endl;
            std::cout << "mask: " << mask << std::endl;
            std::cout << "offset: " << offset << std::endl;
            return 0;
        }
    }
    catch(std::exception &e)
    {
        std::cout << "[Error] " << e.what() << std::endl;
    }

    return 0;
}
