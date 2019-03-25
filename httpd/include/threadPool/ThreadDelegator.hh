#pragma once

#include <functional>
#include <vector>
#include "SocketQueue.hpp"
#include "exception/thread_exception.hh"
#include "ThreadGuard.hh"

namespace zia
{
    class ThreadDelegator
    {
    public:
        using ptr_thread_t = std::unique_ptr<ThreadGuard>;
        using fnc_thread = std::function<void(ISocket *)>;
        using ptr_queue_t = std::unique_ptr<TaskQueue<ISocket *>>;

    public:
        ThreadDelegator(ptr_queue_t, fnc_thread const &, std::size_t const = 80);
        ~ThreadDelegator();
        ThreadDelegator(ThreadDelegator const &) = delete;

        ThreadDelegator &operator=(ThreadDelegator const &) = delete;
        void delegateTask(ISocket *);
        std::size_t getNbrWorkers() const;
        bool isFull() const;
        void launchThreadById(std::size_t , std::function<void(ISocket *)> const &);
        void killWorkers();
        std::size_t sizeTask() const;

    private:
        void m_create_thread(ISocket *);
        void m_flush_queue();
        void m_clear_workers();

    private:
        std::size_t               maxSize_;
        std::vector<ptr_thread_t> workers_;
        ptr_queue_t               tasks_;
        fnc_thread                f_;
    };
}