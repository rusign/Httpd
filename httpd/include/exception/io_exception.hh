#pragma once

#include "custom_exception.hh"

namespace zia
{
    class io_exception : public custom_exception
    {
    public:
        io_exception(std::string const &what) : custom_exception{what} {};
    };
}