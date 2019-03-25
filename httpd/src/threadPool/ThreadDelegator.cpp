#include <utility>
#include <algorithm>
#include <sstream>
#include "threadPool/ThreadDelegator.hh"

namespace zia
{
    ThreadDelegator::ThreadDelegator(ThreadDelegator::ptr_queue_t taskQueue,
                                     ThreadDelegator::fnc_thread const &funcThread,
                                     ::size_t const maxWorkers)
            : maxSize_{maxWorkers}, f_(funcThread)
    {
        tasks_ = std::move(taskQueue);
    }

    ThreadDelegator::~ThreadDelegator()
    {
        killWorkers();
    }

    void ThreadDelegator::delegateTask(ISocket *sock)
    {
        m_clear_workers();
        m_flush_queue();
        if (maxSize_ == workers_.size())
            tasks_->pushInQueue(sock);
        else
            m_create_thread(sock);
    }

    void ThreadDelegator::m_create_thread(ISocket *sock)
    {
        std::size_t id = workers_.empty() ? 1 : workers_.back()->getId() + 1;
        workers_.push_back(std::make_unique<ThreadGuard>(id, sock));
        launchThreadById(id, f_);
    }

    void ThreadDelegator::killWorkers()
    {
        std::for_each(workers_.begin(), workers_.end(),
                      [](ThreadDelegator::ptr_thread_t &thread) { thread->killThread(); });
        workers_.clear();
    }

    void ThreadDelegator::m_flush_queue()
    {
        m_clear_workers();
        while (!tasks_->isEmpty())
        {
            if (workers_.size() == maxSize_)
                return;
            m_create_thread(tasks_->popInQueue());
        }
    }

    void ThreadDelegator::m_clear_workers()
    {
        auto size = workers_.size();
        for (auto i = 0; i < size; ++i)
            if (!workers_[i]->isAlive())
            {
                workers_[i]->killThread();
                workers_.erase(workers_.begin() + i);
                --i;
                --size;
            }
    }

    void ThreadDelegator::launchThreadById(std::size_t id, std::function<void(ISocket *)> const &f)
    {
        auto it = std::find_if(workers_.begin(), workers_.end(),
                               [id](ThreadDelegator::ptr_thread_t &worker) { return worker->getId() == id; });
        if (it == workers_.end())
            throw thread_exception{"thread_exception thrown : id not found"};
        if ((*it)->isLaunched())
            return;
        (*(*it))(f);
    }

    std::size_t ThreadDelegator::sizeTask() const
    {
        return tasks_->size();
    }

    bool ThreadDelegator::isFull() const
    {
        return maxSize_ == workers_.size();
    }

    std::size_t ThreadDelegator::getNbrWorkers() const
    {
        return workers_.size();
    }
}