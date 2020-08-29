#include <locale>
#include <sqlite3.h>
#include "modules/sigmaker.h"
#include "modules/win-api-process-memory-editor.h"
#include "memhax.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <chrono>
#include <thread>
#include <windows.h>
#include "modules/aob-sig.h"

namespace po = boost::program_options;


int main(int argc, char **argv)
{
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

    auto path_to_config = vm["config"].as<std::string>();

    try
    {
        if (!vm.count("command"))
        {
            std::cout << "specify command via --command option" << std::endl;
            return 1;
        }

        auto command = vm["command"].as<std::string>();

        SigmakerConfig cfg(path_to_config);
        WinApiProcessMemoryEditor mem(cfg.getExecutableName());
        SigMaker s(cfg, &mem);

        if (command.compare("append") == 0)
        {
            s.appendSample();
            std::cout << "appended new sample" << std::endl;
            return 0;
        }

        if (command.compare("generate") == 0)
        {
            auto sig = s.generateSignature();
            std::cout << sig << std::endl;
            return 0;
        }
    }
    catch(std::exception e)
    {
        std::cout << "[Error] " << e.what() << std::endl;
    }

    return 0;
}
