#pragma once

#define DEFAULT_PORT    8080

#include <string>

namespace zia
{
    class IHttpServer
    {
    public:
        virtual bool run(const std::string &) = 0;
    };
}

#ifdef _WIN32
    #include "server/HttpServerWindows.hh"
#else
    #include "server/HttpServerUnix.hh"
#endif