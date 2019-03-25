#pragma once

#include <memory>
#include <thread>
#include <functional>
#include "SocketQueue.hpp"

namespace zia
{
    class ThreadGuard
    {
    public:
        ThreadGuard(std::size_t, ISocket *);
        ThreadGuard &operator()(std::function<void(ISocket *)> const &);
        std::size_t getId() const;
        bool isAlive() const;
        void killThread();
        bool isLaunched() const;

    private:
        std::size_t                  id_;
        std::unique_ptr<std::thread> guard_;
        ISocket                      *sock_;
        bool                         isAlive_;
        bool                         isLaunched_;
    };
}