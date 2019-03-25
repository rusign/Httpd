#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <HttpRequest/IHttpRequest.hh>

namespace utils
{
    class Splitter
    {
    public:
        const std::string split(std::string &, const std::string &);
    };
}