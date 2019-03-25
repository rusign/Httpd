#pragma once

#include <Logger/Logger.hh>
#include "exception/custom_exception.hh"
#include "utils/DateTime.hh"

namespace zia
{
    class Logger : public apouche::Logger
    {
    public:
        Logger(bool stacktrace) : stacktrace_(stacktrace) { }
        virtual ~Logger() {};
        void exception(const std::string &, const zia::custom_exception &);
        void debug(const std::string &) override;
        void error(const std::string &) override;
        void fatal(const std::string &) override;
        void info(const std::string &) override;
        void warning(const std::string &) override;
        virtual void write(const std::string &) override;

    private:
        bool stacktrace_;
    };
}