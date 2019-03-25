#include <gtest/gtest.h>
#include "exception/custom_exception.hh"

namespace test
{
    class Exception
    {
    public:
        static void f1(int run) TRACE_EXCEPTION
        ({
             if (run > 32)
                 throw zia::custom_exception("Stacktrace");
             if (run % 2)
                 f2(run + 1);
             else
                 f3(run + 1);
         })

        static void f2(int run) TRACE_EXCEPTION
        ({
             if (!(run % 3))
                 f1(run + 1);
             else
                 f2(run + 1);
         })

        static void f3(int run) TRACE_EXCEPTION
        ({
             f2(run + 1);
         })
    };
}

TEST(Exception, Stacktrace)
{
    try
    {
        test::Exception::f1(0);
    }
    catch (zia::custom_exception &e)
    {
        EXPECT_EQ(35, e.stackTrace().size());
    }
}