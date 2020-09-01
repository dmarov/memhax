#include <locale>
#include <sqlite3.h>
#include "modules/sigmaker.h"
#include "modules/win-api-process-memory-editor.h"
#include "main.h"
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
            ("config", po::value<std::string>(), "specify config");

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
        SigmakerConfig cfg(path_to_config);
        ProcessMemoryEditor* mem = new WinApiProcessMemoryEditor(cfg.getExecutableName());
        SigMaker s(cfg, mem);

        std::cout << "press F9 to start monitor" << std::endl;

        while (true)
        {
            SHORT key_state = GetAsyncKeyState(VK_F9);
            bool key_down = (key_state & 0x8000) && (key_state & 0x0001);

            if (key_down)
            {
                is_monitoring = !is_monitoring;

                if (is_monitoring)
                {
                    std::cout << "monitoring enabled" << std::endl;
                }
                else
                {
                    std::cout << "monitoring disabled" << std::endl;
                }
            }

            if (is_monitoring)
            {
                try
                {
                    s.appendSample();
                    std::cout << "sample added" << std::endl;
                }
                catch(std::exception &e)
                {
                    std::cout << "[Error] " << e.what() << std::endl;
                }

            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    catch(std::exception &e)
    {
        std::cout << "[Error] " << e.what() << std::endl;
    }

    return 0;
}
