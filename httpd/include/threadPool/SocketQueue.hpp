#pragma once

#include <queue>
#include "threadPool/TaskQueue.hpp"

namespace zia
{
    class SocketQueue : public zia::TaskQueue<zia::ISocket *>
    {
    public:
        SocketQueue(std::mutex &m) : zia::TaskQueue<zia::ISocket *>{m} {};
        SocketQueue() : zia::TaskQueue<zia::ISocket *>{} {};
        bool isEmpty() const override { return queue_.empty(); };
        std::size_t size() const override { return queue_.size(); };

    private:
        void pushInQueueImpl(zia::ISocket *sock) override { queue_.push(sock); }
        zia::ISocket *popInQueueImpl() override
        {
            zia::ISocket *sock = queue_.front();
            queue_.pop();
            return sock;
        }

    private:
        std::queue<zia::ISocket *> queue_;
    };
}