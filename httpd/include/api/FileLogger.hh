#pragma once

#include <stdio.h>
#include <fstream>
#include "Logger.hh"

namespace zia
{
    class FileLogger : public Logger
    {
    public:
        FileLogger(const std::string &, bool);
        virtual ~FileLogger();
        virtual void write(const std::string &);
        void clearFile();

    private:
        std::string   _filename;
        std::ofstream _file;
    };
}