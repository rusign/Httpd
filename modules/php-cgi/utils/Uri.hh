#pragma once

#include <string>

namespace utils
{
    class Uri
    {
    public:
        std::string decode(const std::string &) const;
        std::string extractUrl(const std::string &) const;
    };
}