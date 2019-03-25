#pragma once

#include "custom_exception.hh"

namespace zia
{
    class splitter_exception : public custom_exception
    {
    public:
        splitter_exception(std::string const &what) : custom_exception{what} {};
    };
}