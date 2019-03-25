#pragma once

#include <mutex>
#include <type_traits>
#include <cassert>
#include <HttpMessage/IHttpMessage.hh>
#include "network/ISocket.h"

namespace zia
{
    template<typename T>
    class TaskQueue;

    template<typename T>
    class TaskQueue<T *>
    {
        static_assert(std::is_base_of<ISocket, T>::value, "T must be inherit from ISocket");
    public:
        TaskQueue(std::mutex &m) : m_{&m} {}
        TaskQueue() : m_{nullptr} {}
        void pushInQueue(T *lhs);
        T *popInQueue();
        virtual bool isEmpty() const = 0;
        virtual std::size_t size() const = 0;

    private:
        virtual void pushInQueueImpl(T *lhs) = 0;
        virtual T *popInQueueImpl() = 0;

    private:
        std::mutex *m_;
    };

    template<typename T>
    void TaskQueue<T *>::pushInQueue(T *lhs)
    {
        if (m_)
            std::lock_guard<std::mutex>(*(this->m_));
        pushInQueueImpl(lhs);
    }

    template<typename T>
    T *TaskQueue<T *>::popInQueue()
    {
        assert(!isEmpty() && "Error : trying to pop an empty queue");
        if (m_)
            std::lock_guard<std::mutex>(*(this->m_));
        return popInQueueImpl();
    }
}