#include <iostream>
#include <boost/program_options.hpp>
#include "main.h"
#include "injection-methods/create-remote-thread-method.h"
#include "injection-methods/generic-method.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    po::options_description desc("Supported options");

    desc.add_options()
        ("help", "produce help message")
        ("version,v", "print version")
        ("lib", po::value<std::string>()
            ->required(),
        "specify library to inject via --lib")
        ("target", po::value<std::string>()
            ->required(),
        "specify target process name via --target")
        ("method", po::value<std::string>()
            ->required()
            ->implicit_value("create-remote-thread")
            ->implicit_value("manual-mapping")
            ->default_value("create-remote-thread"),
        "specify injection method");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("version"))
    {
        std::cout << "v" << dll_injector_VERSION_MAJOR << "."
            << dll_injector_VERSION_MINOR << "."
            << dll_injector_VERSION_PATCH << std::endl;
        return 0;
    }

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    GenericMethod* method = NULL;

    auto target_name = vm["target"].as<std::wstring>();
    auto library_file = vm["lib"].as<std::wstring>();
    auto method_str = vm["method"].as<std::string>();

    if (method_str.compare("create-remote-thread"))
    {
        method = new CreateRemoteThreadMethod;
    }

    try
    {
        if (method != NULL)
        {
            method->inject(target_name, library_file);
        }

        std::cout << "Injected successfully" << std::endl;
    }
    catch(std::exception e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
