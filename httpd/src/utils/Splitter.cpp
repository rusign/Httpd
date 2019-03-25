#include "utils/Splitter.hh"

namespace zia
{
    namespace utils
    {
        const std::string zia::utils::Splitter::split(std::string &source, const std::string &delim)
        {
            std::string head(source);

            auto npos = source.find(delim);
            if (npos == std::string::npos)
            {
                source = "";
                return head;
            }
            head      = source.substr(0, npos);
            source    = source.substr(npos + delim.size());
            return head;
        }
    }
}