#include "api/FileLogger.hh"

zia::FileLogger::FileLogger(const std::string &str, bool stacktrace) : Logger(stacktrace)
{
    _file.open(str, std::ios::out | std::ios::app);
}

zia::FileLogger::~FileLogger()
{
    if (_file.is_open())
        _file.close();
}

void zia::FileLogger::write(const std::string &message)
{
    if (_file.is_open())
        _file << message << std::endl;
}

void zia::FileLogger::clearFile()
{
    if (_file.is_open())
        _file.clear();
}
