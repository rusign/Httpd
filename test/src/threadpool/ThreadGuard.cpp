#include <gtest/gtest.h>
#include "../tools/SocketImpl.hpp"
#include "threadPool/ThreadGuard.hh"
#include "../tools/TaskQueue.hpp"

static void triplet()
{
    test::SocketImplTest sockTest1{};
    test::SocketImplTest sockTest2{};
    test::SocketImplTest sockTest3{};

    zia::ThreadGuard t1{1, &sockTest1};
    zia::ThreadGuard t2{2, &sockTest2};
    zia::ThreadGuard t3{3, &sockTest3};

    auto f1 = [](zia::ISocket *sock)
    {
        std::string buff;
        sock->ReceiveRequest();
    };
    auto f2 = [](zia::ISocket *sock)
    {
        std::string buff;
        sock->ReceiveRequest();
    };
    auto f3 = [](zia::ISocket *sock)
    {
        std::string buff;
        sock->ReceiveRequest();
    };

    t1(f1);
    t2(f2);
    t3(f3);

    EXPECT_EQ(1, t1.getId()) << "Error, bad ids";
    EXPECT_EQ(2, t2.getId()) << "Error, bad ids";
    EXPECT_EQ(3, t3.getId()) << "Error, bad ids";

    t1.killThread();
    t2.killThread();
    t3.killThread();

    EXPECT_FALSE(t1.isAlive()) << "Error, all threads are supposed to be dead";
    EXPECT_FALSE(t2.isAlive()) << "Error, all threads are supposed to be dead";
    EXPECT_FALSE(t3.isAlive()) << "Error, all threads are supposed to be dead";
}

TEST(ThreadPool, ThreadGuard_Triplet)
{
    triplet();
}

TEST(ThreadPool, ThreadGuard_Overkill)
{
    for (std::size_t i = 0; i < 10000; ++i)
        triplet();
}

TEST(ThreadPool, ThreadGuard_ChuckNorris)
{
    for (std::size_t i = 0; i < 1000; ++i)
    {
        std::vector<test::SocketImplTest> sockets;
        std::vector<zia::ThreadGuard>     guards;

        for (std::size_t j = 0; j < 200; ++j)
        {
            sockets.push_back(test::SocketImplTest{});
            guards.push_back(zia::ThreadGuard{j, &sockets[j]});
        }
        for (auto        j = 0; j < 200; ++j)
        {
            auto f = [](zia::ISocket *sock) {};
            guards[j](f);
            EXPECT_EQ(j, guards[j].getId()) << "Error, bad ids";
        }
        for (auto        j = 0; j < 200; ++j)
            guards[j].killThread();
        for (auto        j = 0; j < 200; ++j)
            EXPECT_FALSE(guards[j].isAlive()) << "Error, all threads are supposed to be dead";

    }
}