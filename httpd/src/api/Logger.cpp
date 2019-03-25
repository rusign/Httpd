#include "api/Logger.hh"

void zia::Logger::exception(const std::string &src, const zia::custom_exception &e)
{
    std::stringstream ss;
    ss << "[" << zia::utils::DateTime().currentTime() << "]" << " [ ERROR ] [" << src << "] " << e.what();
    if (stacktrace_)
    {
        ss << std::endl;
        for (auto trace : e.stackTrace())
            ss << "\t\tat " << trace << std::endl;
    }
    write(ss.str());
}


void zia::Logger::debug(const std::string &message)
{
    std::stringstream ss;
    ss << "[" << zia::utils::DateTime().currentTime() << "]" << " [ DEBUG ] " << message;
    write(ss.str());
}

void zia::Logger::error(const std::string &message)
{
    std::stringstream ss;
    ss << "[" << zia::utils::DateTime().currentTime() << "]" << " [ ERROR ] " << message;
    write(ss.str());
}

void zia::Logger::fatal(const std::string &message)
{
    std::stringstream ss;
    ss << "[" << zia::utils::DateTime().currentTime() << "]" << " [ FATAL ] " << message;
    write(ss.str());
}

void zia::Logger::info(const std::string &message)
{
    std::stringstream ss;
    ss << "[" << zia::utils::DateTime().currentTime() << "]" << " [ INFO  ] " << message;
    write(ss.str());
}

void zia::Logger::warning(const std::string &message)
{
    std::stringstream ss;
    ss << "[" << zia::utils::DateTime().currentTime() << "]" << " [WARNING] " << message;
    write(ss.str());
}

void zia::Logger::write(const std::string &message)
{
    if (message.find("[WARNING]") != std::string::npos ||
        message.find("[ FATAL ]") != std::string::npos ||
        message.find("[ ERROR ]") != std::string::npos)
        std::cerr << message << std::endl;
    else
        std::cout << message << std::endl;
}
