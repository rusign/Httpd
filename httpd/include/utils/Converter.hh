#pragma once

#include <Enum.hh>

namespace zia
{
    namespace utils
    {
        class Converter
        {
        public:
            const apouche::StatusCode toStatusCode(const std::string &) const;
            const apouche::Method toMethod(const std::string &) const;
            const uint64_t toDate(const std::string &) const;

            const std::string &toString(apouche::Method) const;
            const std::string &toString(apouche::StatusCode) const;
        };
    }
}