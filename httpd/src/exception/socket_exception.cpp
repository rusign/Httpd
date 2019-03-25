#include "exception/socket_exception.hh"

namespace zia
{
    socket_exception::socket_exception(std::string const &what) : custom_exception{"SocketError: " + what} {}
}