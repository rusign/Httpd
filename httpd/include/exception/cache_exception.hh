#pragma once

#include "custom_exception.hh"

namespace zia
{
    class cache_exception : public custom_exception
    {
    public:
        cache_exception(std::string const &what) : custom_exception{what} {}
    };
}