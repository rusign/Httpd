#include <gtest/gtest.h>
#include "../tools/SocketImpl.hpp"
#include "../tools/TaskQueue.hpp"

void pushTask(zia::TaskQueue<zia::ISocket *> &tasks)
{
    for (std::size_t i = 0; i < 40; ++i)
        tasks.pushInQueue(new test::SocketImplTest{});
    EXPECT_FALSE(tasks.isEmpty()) << "Error : taskQueue not supposed to be empty";
    EXPECT_EQ(40, tasks.size()) << "Error : taskQueue supposed to have a length of 40";
}

void popTask(zia::TaskQueue<zia::ISocket *> &tasks)
{
    std::size_t lastSize{tasks.size()};
    while (!tasks.isEmpty())
    {
        zia::ISocket *sock = tasks.popInQueue();
        EXPECT_LT(tasks.size(), lastSize) << "Error : taskQueue supposed to have a size smaller than expected";

        sock->ReceiveRequest();
        auto recv = sock->getNextRequest();

        EXPECT_EQ(nullptr, recv) << "Error : socket supposed to return Hi!";
        lastSize = tasks.size();
    }
    EXPECT_EQ(0, lastSize) << "Error : Size supposed to be 0";
}

void sizeQueue()
{
    std::mutex m{};

    zia::TaskQueue<zia::ISocket *> *t1 = new test::sockQueue{m};
    zia::TaskQueue<zia::ISocket *> *t2 = new test::sockQueue{m};
    zia::TaskQueue<zia::ISocket *> *t3 = new test::sockQueue{m};

    t1->pushInQueue(new test::SocketImplTest());
    t2->pushInQueue(new test::SocketImplTest());
    t3->pushInQueue(new test::SocketImplTest());

    EXPECT_FALSE(t1->isEmpty()) << "Error : no are supposed to be empty";
    EXPECT_FALSE(t2->isEmpty()) << "Error : no are supposed to be empty";
    EXPECT_FALSE(t3->isEmpty()) << "Error : no are supposed to be empty";
    EXPECT_EQ(t1->size(), t2->size()) << "Error : all the queues are supposed to have the same size";
    EXPECT_EQ(t1->size(), t3->size()) << "Error : all the queues are supposed to have the same size";

    zia::ISocket *sock = t1->popInQueue();
    EXPECT_NE(nullptr, dynamic_cast<test::SocketImplTest *>(sock));
    EXPECT_LT(t1->size(), t2->size()) << "Error : t1 supposed to have a smaller size than t2";
    EXPECT_TRUE(t1->isEmpty()) << "Error : t1 supposed to be empty";

    delete t1;
    delete t2;
    delete t3;
}

TEST(ThreadPool, TaskQueue)
{
    test::sockQueue sock_test{};
    pushTask(sock_test);
    popTask(sock_test);
    sizeQueue();
}