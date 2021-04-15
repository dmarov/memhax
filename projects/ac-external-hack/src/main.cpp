#include <chrono>
#include <iostream>
#include "modules/win-api-external-process-memory-editor.h"
#include "loops/ac-hack-loop.h"
#include <boost/program_options.hpp>
#include "main.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{

    po::options_description desc("Supported options");

    desc.add_options()
        ("help", "produce help message")
        ("version,v", "print version");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("version"))
    {
        std::cout << "v" << ac_hack_VERSION_MAJOR << "." << ac_hack_VERSION_MINOR << "." << ac_hack_VERSION_PATCH << std::endl;
        return 0;
    }

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    const wchar_t* exe = L"ac_client.exe";
    memhax::WinApiExternalProcessMemoryEditor editor(exe);

    std::cout << "Searching for signatures..." << std::endl;

    try
    {
        AcHackLoop loop(editor);
        loop.start();
    }
    catch(std::exception &e)
    {
        std::cerr << "error occured: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

