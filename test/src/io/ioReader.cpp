#include <thread>
#include <gtest/gtest.h>
#include "io/ioReader.hh"
#include "io/ioWriter.hh"

TEST(IO, Simple)
{
    std::mutex        m;
    zia::io::ioReader r{"iofile", m};
    zia::io::ioWriter w{"iofile", m};

    std::thread t1{[&w]()
                   {
                       std::string a{};
                       for (auto   i = 0; i < 1048576; ++i)
                           a += "a";
                       w.writeInFile(a + std::string{"\n"});
                   }};
    std::thread t2{[&r]()
                   {
                       std::string b{};
                       for (auto   i = 0; i < 1048576; ++i)
                           b += "a";
                       b += "\n";
                       auto str = r.readFromFile();
                   }};

    t1.join();
    t2.join();
}

TEST(IO, Stability)
{
    std::mutex               m;
    std::vector<std::thread> threads;
    zia::io::ioReader        r{"iofile_", m};
    zia::io::ioWriter        w{"iofile_", m};

    for (int i = 0; i < 2; ++i)
    {
        threads.push_back(std::thread{[&w]()
                                      {
                                          std::string a{};
                                          for (auto   i = 0; i < 256; ++i)
                                              a += "a";
                                          w.writeInFile(a + std::string{"\n"});
                                      }});
        threads.push_back(std::thread{[&r]()
                                      {
                                          std::string b{};
                                          for (auto   i = 0; i < 256; ++i)
                                              b += "a";
                                          b += "\n";
                                          auto str = r.readFromFile();
                                      }});
    }

    for (auto it = threads.begin(); it != threads.end(); ++it)
        it->join();
}
