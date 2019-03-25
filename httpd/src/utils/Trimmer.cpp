#include <algorithm>
#include <cctype>
#include <functional>
#include "utils/Trimmer.hh"

std::string &zia::utils::Trimmer::ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
std::string &zia::utils::Trimmer::rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
std::string &zia::utils::Trimmer::trim(std::string &s)
{
    return ltrim(rtrim(s));
}
