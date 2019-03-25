#ifdef _WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4552)
#endif

#include <sstream>
#include <iomanip>
#include "utils/Splitter.hh"
#include "utils/Uri.hh"

namespace zia
{
    namespace utils
    {
        const std::string Uri::regex_ = Uri::regexValidator();

        std::string Uri::encode(const std::string &str) const
        {
            std::ostringstream escaped;
            escaped.fill('0');
            escaped << std::hex;

            for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i)
            {
                std::string::value_type c = (*i);
                if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
                {
                    escaped << c;
                    continue;
                }
                escaped << std::uppercase;
                escaped << '%' << std::setw(2) << int((unsigned char) c);
                escaped << std::nouppercase;
            }
            return escaped.str();
        }

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
            Splitter splitter;
            std::string uri_cpy(uri);

            auto url  = splitter.split(uri_cpy, "?");
            return decode(url);
        }

        const std::string Uri::regexValidator()
        {
            std::string url_regex = "^";

            // protocol identifier
            url_regex += "(?:(?:https?|ftp)://)?";

            // user:pass authentication
            url_regex += "(?:\\S+(?::\\S*)?@)?";

            // hostname
            url_regex += "(?:";
            url_regex += "(?:";

            // IP address exclusion
            // private & local networks
            url_regex += "(?!(?:10|127)(?:\\.\\d{1,3}){3})";
            url_regex += "(?!(?:169\\.254|192\\.168)(?:\\.\\d{1,3}){2})";
            url_regex += "(?!172\\.(?:1[6-9]|2\\d|3[0-1])(?:\\.\\d{1,3}){2})";

            // IP address dotted notation octets
            // excludes loopback network 0.0.0.0
            // excludes reserved space >= 224.0.0.0
            // excludes network & broacast addresses
            // (first & last IP address of each class)
            url_regex += "(?:[1-9]\\d?|1\\d\\d|2[01]\\d|22[0-3])";
            url_regex += "(?:\\.(?:1?\\d{1,2}|2[0-4]\\d|25[0-5])){2}";
            url_regex += "(?:\\.(?:[1-9]\\d?|1\\d\\d|2[0-4]\\d|25[0-4]))";
            url_regex += "|";

            // host name
            url_regex += "(?:(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)";
            // domain name
            url_regex += "(?:\\.(?:[a-z\\u00a1-\\uffff0-9]-*)*[a-z\\u00a1-\\uffff0-9]+)*";
            // TLD identifier;
            url_regex += "(?:\\.(?:[a-z\\u00a1-\\uffff]{2,}))?";
            // TLD may end with dot
            url_regex += "\\.?";
            url_regex += ")";

            // port number;
            url_regex += "(?::\\d{2,5})?";

            // end hostname
            url_regex += ")?";

            // resource path;
            url_regex += "(?:[/?#]\\S*)?";

            return url_regex;
        }
    }
}