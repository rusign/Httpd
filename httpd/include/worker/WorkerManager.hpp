#pragma once

#include "httpEngine/Engine.hh"
#include "threadPool/ThreadDelegator.hh"
#include "worker/Worker.hh"

namespace zia
{
    class WorkerManager : public zia::ThreadDelegator
    {
    public:
        WorkerManager(ThreadDelegator::ptr_queue_t tasks, std::mutex &guard,
                      zia::Logger &logger, std::size_t const maxSize = 80)
                : zia::ThreadDelegator(std::move(tasks), [&guard, &logger](ISocket *s)
        {
            try { zia::Worker().treat(s, &guard, &logger); }
            catch (...) { s->disconnect(); }
        }, maxSize) {}
		WorkerManager(const WorkerManager &) = delete;
    };
}