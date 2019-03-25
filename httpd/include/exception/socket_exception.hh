#pragma once

#include "exception/custom_exception.hh"

namespace zia
{
    class socket_exception : public zia::custom_exception
    {
    public:
        socket_exception(std::string const &);
    };
}