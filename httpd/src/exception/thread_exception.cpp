#include "exception/thread_exception.hh"

namespace zia
{
    thread_exception::thread_exception(std::string const &what) : custom_exception{what} {}
}