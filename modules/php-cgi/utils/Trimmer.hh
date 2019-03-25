#pragma once

#include <string>

namespace utils
{
    class Trimmer
    {
    public:
        std::string &ltrim(std::string &);
        std::string &rtrim(std::string &);
        std::string &trim(std::string &);
    };
}
