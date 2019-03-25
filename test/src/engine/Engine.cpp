#ifdef WIN32
#include <direct.h>
#define getcwd _getcwd
#else

#include <unistd.h>

#endif

#include <iostream>
#include <mutex>
#include <thread>
#include <gtest/gtest.h>
#include "api/HttpRequest.hpp"
#include "api/HttpResponse.hpp"
#include "httpEngine/Engine.hh"
#include "threadPool/ThreadDelegator.hh"

#include "../tools/ConfImpl.hpp"
#include "../tools/LoggerImpl.hpp"
#include "../tools/RequestGenerator.hpp"
#include "../tools/TaskQueue.hpp"
#include "../tools/SocketImpl.hpp"

using namespace zia;

static std::string pwd()
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

static test::ConfImpl *newConf(const std::string &path)
{
    auto conf = new test::ConfImpl;
    auto root = pwd() + "/ressources";

    conf->setConf("sites:localhost:directory", root + path);
    conf->setConf("sites:\"*\":directory", root + "/*" + path);
    return conf;
}

TEST(Engine, Simple)
{
/*    std::mutex m;
    auto       conf    = newConf("");
    auto       engine  = zia::httpEngine::Engine(m, conf);
    auto       request = test::RequestGenerator::simple_get();
    auto       logger  = test::LoggerImpl();

    auto response = engine(request, logger);
    EXPECT_EQ(test::RequestGenerator::simple_get_response(), response.getBody()->getBody()) << "Responses don't match";

    delete conf;*/
}

TEST(Engine, Multithread)
{
/*    std::mutex               lock;
    auto                     conf   = newConf("");
    auto                     engine = zia::httpEngine::Engine(lock, conf);
    auto                     logger = test::LoggerImpl();
    std::vector<std::thread> threads;

    for (int i = 0; i < 1024; ++i)
    {
        threads.push_back(std::thread{[&]()
                                      {
                                          zia::HttpRequest requests[2]  =
                                                                   {
                                                                           {test::RequestGenerator::simple_get()},
                                                                           {test::RequestGenerator::simple_get_with_query()}
                                                                   };
                                          std::string      responses[2] =
                                                                   {
                                                                           {test::RequestGenerator::simple_get_response()},
                                                                           {test::RequestGenerator::simple_get_with_query_response()}
                                                                   };

                                          auto idx     = std::rand() % 2;
                                          auto request = requests[idx];

                                          auto response = engine(request, logger);
                                          EXPECT_EQ(responses[idx], response.getBody()->getBody())
                                                              << "Responses don't match (Thread #" + std::to_string(i) +
                                                                 ")";
                                      }});
    }

    for (auto it = threads.begin(); it < threads.end(); ++it)
        it->join();
    delete conf;
    */
}
