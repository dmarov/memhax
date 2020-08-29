#include "sigmaker-config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <yaml-cpp/yaml.h>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

using boost::uuids::detail::md5;

SigmakerConfig::SigmakerConfig(std::string path_to_file)
{
    try
    {
        YAML::Node config = YAML::LoadFile(path_to_file);
        std::string module_name = config["module"].as<std::string>();
        std::wstring wmodule_name(module_name.begin(), module_name.end());
        this->module_name = wmodule_name;
        std::string executable_name = config["executable"].as<std::string>();
        std::wstring wexecutable_name(executable_name.begin(), executable_name.end());
        this->executable_name = wexecutable_name;
        this->size = config["size"].as<unsigned int>();
        auto offsets = config["offsets"].as<std::vector<std::string>>();
        this->offsets = std::vector<size_t>();
        this->offset = -50;
        this->len = 100;

        if (config["offset"])
        {
            this->offset = config["offset"].as<unsigned int>();
        }

        if (config["length"])
        {
            this->len = config["length"].as<unsigned int>();
        }

        for(auto offset : offsets)
        {
            unsigned long offset_int;
            std::stringstream ss;

            ss << std::hex << offset;
            ss >> offset_int;
            this->offsets.push_back(offset_int);
        }

        std::ifstream file(path_to_file);
        file.seekg( 0, std::ios::end );
        size_t size = file.tellg();
        char* bytes = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(bytes, len);

        md5 hash;
        md5::digest_type digest;

        hash.process_bytes(bytes, size);
        hash.get_digest(digest);

        const auto char_digest = reinterpret_cast<const char *>(&digest);
        std::string md5_hash;
        boost::algorithm::hex(char_digest, char_digest + sizeof(md5::digest_type), std::back_inserter(md5_hash));
        this->session_id = md5_hash;

    }
    catch(std::exception &e)
    {
        std::cerr << "failed to parse config" << std::endl;
        throw e;
    }
}

SigmakerConfig::SigmakerConfig() {}

std::wstring SigmakerConfig::getExecutableName()
{
    return this->executable_name;
}

std::string SigmakerConfig::getSessionId()
{
    return this->session_id;
}

std::wstring SigmakerConfig::getModuleName()
{
    return this->module_name;
}

std::vector<size_t> SigmakerConfig::getOffsets()
{
    return this->offsets;
}

unsigned SigmakerConfig::getSize()
{
    return this->size;
}

int SigmakerConfig::getOffset()
{
    return this->offset;
}

unsigned SigmakerConfig::getLength()
{
    return this->len;
}
