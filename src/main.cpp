#include <locale>
#include <sqlite3.h>
#include "modules/sigmaker.h"
#include "main.h"
#include <iostream>
#include <boost/program_options.hpp>

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
        std::cout << "v" << sigmaker_VERSION_MAJOR << "." << sigmaker_VERSION_MINOR << "." << sigmaker_VERSION_PATCH << std::endl;
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
        return 0;
    }

    if (command.compare("generate") == 0)
    {
        std::cout << SigMaker::generateSignature(config) << std::endl;
        return 0;
    }

    return 0;
}
