#pragma once

#include <regex>

namespace zia
{
    namespace utils
    {
        class Uri
        {
        public:
            std::string encode(const std::string &) const;
            std::string decode(const std::string &) const;
            std::string extractUrl(const std::string &) const;
            bool validate(const std::string &url) { return std::regex_match(url, std::regex(regex_)); }

        private:
            static const std::string regexValidator();

        private:
            static const std::string regex_;
        };
    }
}