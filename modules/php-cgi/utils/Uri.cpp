#ifdef _WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4552)
#endif

#include "Splitter.hh"
#include "Uri.hh"

namespace utils
{
    std::string Uri::decode(const std::string &str) const
    {
        std::string ret;

        for (size_t i = 0; i < str.length(); i++)
            if (str[i] == '%')
            {
                int decoded;

                sscanf(str.substr(i + 1, 2).c_str(), "%x", &decoded);
                ret += static_cast<char>(decoded);
                i += 2;
            }
            else
                ret += str[i];
        return ret;
    }

    std::string Uri::extractUrl(const std::string &uri) const
    {
        Splitter    splitter;
        std::string uri_cpy(uri);

        auto url = splitter.split(uri_cpy, "?");
        url = "/" + url;
        return decode(url);
    }
}