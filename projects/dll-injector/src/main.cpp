#include <iostream>
#include <boost/program_options.hpp>
#include "boost/program_options/parsers.hpp"
#include "main.h"
#include "injection-methods/create-remote-thread-method.h"
#include "injection-methods/generic-method.h"

namespace po = boost::program_options;

int wmain(int argc, wchar_t *argv[])
{

    try
    {
        po::options_description desc("Supported options");

        desc.add_options()
            ("help", "produce help message")
            ("version,v", "print version")
            ("lib", po::wvalue<std::wstring>()
                ->required(),
                "specify library to inject via --lib")
            ("target", po::wvalue<std::wstring>()
                ->required(),
                "specify target process name via --target")
            ("method", po::wvalue<std::wstring>()
                ->required()
                ->default_value(L"create-remote-thread", "create-remote-thread"),
                "specify injection method");

        po::variables_map vm;
        po::store(
            po::wcommand_line_parser(argc, argv)
                .options(desc)
                .run(),
            vm
        );

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
        auto method_str = vm["method"].as<std::wstring>();

        if (!method_str.compare(L"create-remote-thread"))
        {
            method = new CreateRemoteThreadMethod;
        }

        if (method != NULL)
        {
            method->inject(target_name, library_file);
        }
        else
        {
            throw std::exception("invalid injection method");
        }

        std::cout << "Injected successfully" << std::endl;
    }
    catch(std::exception e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
