#include <gtest/gtest.h>
#include "network/ISocket.h"
#include "../tools/TaskQueue.hpp"
#include "../tools/SocketImpl.hpp"
#include "threadPool/ThreadDelegator.hh"

static void thread(zia::ISocket *s)
{
    sleep(1);
    EXPECT_NE(nullptr, s);
}

TEST(ThreadPool, ThreadDelegator_CreateAndKill)
{
    zia::ThreadDelegator t1(std::make_unique<test::sockQueue>(), thread, 200);

    for (std::size_t i = 0; i < 800; ++i)
        t1.delegateTask(new test::SocketImplTest{});

    EXPECT_TRUE(t1.isFull()) << "Error, ThreadDelegator supposed to be full";
    EXPECT_EQ(200, t1.getNbrWorkers()) << "Error, ThreadDelegator supposed to have 200 threads in it";

    t1.killWorkers();
    EXPECT_FALSE(t1.isFull()) << "Errors, ThreadDelegator supposed to be empty, not full";
    EXPECT_EQ(0, t1.getNbrWorkers()) << "Errors, ThreadDelegator supposed to have 0 workers";
}

TEST(ThreadPool, ThreadDelegator_Stability)
{
    zia::ThreadDelegator t1{std::make_unique<test::sockQueue>(), thread, 1000};

    for (std::size_t i = 0; i < 1000; ++i)
        t1.delegateTask(new test::SocketImplTest{});
    EXPECT_TRUE(t1.isFull()) << "Error, ThreadDelegator supposed to be full";
    EXPECT_EQ(1000, t1.getNbrWorkers()) << "Error, ThreadDelegator supposed to have 1000 threads in it";

    t1.killWorkers();
    EXPECT_FALSE(t1.isFull()) << "Errors, ThreadDelegator supposed to be empty, not full";
    EXPECT_EQ(0, t1.getNbrWorkers()) << "Errors, ThreadDelegator supposed to have 0 workers";
}