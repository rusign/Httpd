#pragma once

#include <mutex>
#include "api/Logger.hh"
#include "network/ISocket.h"

namespace zia
{
    class Worker
    {
    public:
        static void treat(ISocket *, std::mutex *, zia::Logger *);
    };
}