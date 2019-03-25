#ifdef _WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning(disable : 4996)
#endif

#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include "httpEngine/Grammar.hh"

#ifdef _WIN32
	#include <Windows.h>
#endif

namespace zia
{
    namespace utils
    {
        class DateTime
        {
        public:
            const std::string currentTime()
            {
                auto now       = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);

                std::stringstream ss;
                ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
                return ss.str();
            }

            const std::string currentHttpTime()
            {
                auto now       = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);

                std::stringstream ss;
                ss << std::put_time(std::gmtime(&in_time_t), zia::httpEngine::Grammar::Date::rfc1123_date.c_str());
                return ss.str();
            }
        };
    }
}