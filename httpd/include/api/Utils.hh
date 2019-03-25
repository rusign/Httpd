#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma once

#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <chrono>
#include <ctime>
#include <sstream>

namespace zia
{
    class Utils
    {
    public:
        static const std::string CurrentTime()
        {
            auto now       = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
            return ss.str();
        }
    };
}