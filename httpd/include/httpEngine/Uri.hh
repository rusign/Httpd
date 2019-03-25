#pragma once

#include "HttpEngine.hh"

namespace zia
{
    namespace httpEngine
    {
        class Uri
        {
        public:
            const std::string encode(const std::string &);
            const std::string decode(const std::string &);
            bool validate(const std::string &);
        };
    }
}