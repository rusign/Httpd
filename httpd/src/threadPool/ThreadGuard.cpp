#include "threadPool/ThreadGuard.hh"

namespace zia
{
    ThreadGuard::ThreadGuard(std::size_t id, ISocket *sock) : id_{id}, sock_{sock}, isAlive_{true}, isLaunched_{false},
                                                              guard_{nullptr} {}

    ThreadGuard &ThreadGuard::operator()(std::function<void(ISocket *)> const &f)
    {
        isLaunched_ = true;
        guard_      = std::make_unique<std::thread>([f, this]()
                                                    {
                                                        f(this->sock_);
                                                        this->isAlive_ = false;
                                                    });
        return *this;
    }

    bool ThreadGuard::isLaunched() const
    {
        return isLaunched_;
    }

    void ThreadGuard::killThread()
    {
        if (isLaunched_)
        {
            if (guard_->joinable())
                guard_->join();
            guard_.reset();
        }
    }

    bool ThreadGuard::isAlive() const
    {
        return isAlive_;
    }

    std::size_t ThreadGuard::getId() const
    {
        return id_;
    }
}