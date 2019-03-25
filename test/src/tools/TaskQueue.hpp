#pragma once

#include <queue>
#include "threadPool/SocketQueue.hpp"

namespace test
{
    class sockQueue : public zia::TaskQueue<zia::ISocket *>
    {
    public:
        sockQueue(std::mutex &m) : zia::TaskQueue<zia::ISocket *>{m} {};
        sockQueue() : zia::TaskQueue<zia::ISocket *>{} {};
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