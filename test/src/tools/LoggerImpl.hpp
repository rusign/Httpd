#include "api/Logger.hh"

namespace test
{
    class LoggerImpl : public zia::Logger
    {
    public:
        LoggerImpl() : zia::Logger(false) {}

        void exception(const std::string &, const zia::custom_exception &) {}
        void debug(const std::string &) override { }
        void error(const std::string &) override { }
        void fatal(const std::string &) override { }
        void info(const std::string &) override { }
        void warning(const std::string &) override { }
        virtual void write(const std::string &) override { }

    };
}