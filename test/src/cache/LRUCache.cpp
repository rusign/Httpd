#include <algorithm>
#include <map>
#include <gtest/gtest.h>
#include "exception/cache_exception.hh"
#include "cache/LRUCache.hpp"

static std::string random_string(size_t length)
{
    auto randchar = []() -> char
    {
        const char   charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz @[]<>'\"\\/!?;:.,+-*=";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };

    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

TEST(LRUCache, SimpleTest)
{
    zia::io::LruCache<int, std::string, std::map> c{10};

    c.put(1, "alpha");
    c.put(2, "beta");
    c.put(3, "charlie");
    auto value = c.retrieveValue(1);
    EXPECT_EQ(value, "alpha");

    auto valueFirst = c.getFirstElement();
    EXPECT_EQ(valueFirst, "beta") << "'beta' is expected on first element";

    c.put(1, "delta");
    EXPECT_EQ(c.retrieveValue(1), "delta") << "'delta' is expected on key 1";

    c.put(5, "a");
    c.put(6, "b");
    c.put(7, "c");
    c.put(8, "d");
    c.put(9, "e");
    c.put(10, "f");
    c.put(11, "g");
    c.put(12, "h");

    value = c.getFirstElement();
    EXPECT_EQ(value, "charlie") << "'charlie' is expected on first element";

    try
    { auto catch_ = c.retrieveValue(1); }
    catch (zia::cache_exception const &e)
    {
        EXPECT_EQ(std::string(e.what()), "Error, key not found in cache") << "Found key, it's weird";
    }

}

TEST(LRUCache, HardTest)
{
    size_t size = 65536;
    auto word = random_string(4096);
    zia::io::LruCache<int, std::string, std::map> c{size};

    for (auto i = 0; i < size; ++i)
        c.put(i, word);

    try
    {
        auto value = c.retrieveValue(0);
        EXPECT_EQ(value, word);
    }
    catch (zia::cache_exception const &e)
    {
        FAIL() << "No exception supposed to be thrown (" << e.what() << ")";
    }
}
