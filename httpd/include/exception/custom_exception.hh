#pragma once

#include <list>
#include <stdexcept>

#ifdef _WIN32
#define __TRACE__ (std::string(__FUNCTION__) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")")
#else
#define __TRACE__ (std::string(__PRETTY_FUNCTION__) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")")
#endif
#define TRACE_EXCEPTION(fnc)    { try { fnc } catch (zia::custom_exception &e) { e << __TRACE__; throw; } }

namespace zia
{
    using StackTrace = std::list<std::string>;

    class custom_exception : public std::exception
    {
    public:
        custom_exception(std::string const &);
        ~custom_exception() throw() = default;
        const char *what() const throw();

        const StackTrace &stackTrace() const { return stackTrace_; }
        const custom_exception &operator<<(const std::string &trace)
        {
            stackTrace_.push_back(trace);
            return *this;
        }

    private:
        StackTrace        stackTrace_;
        std::string const what_;
    };
}