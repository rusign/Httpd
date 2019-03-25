#pragma once

#include "exception/custom_exception.hh"

namespace zia
{
    class thread_exception : public custom_exception
    {
    public:
        thread_exception(std::string const &what);
    };
}