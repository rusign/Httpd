#include "utils/String.hh"

bool zia::utils::String::startWith(const std::string &chain, const std::string &start)
{
    return !chain.empty() && chain.find(start) == 0;
}

bool zia::utils::String::endWith(const std::string &chain, const std::string &end)
{
    return !chain.empty() && chain.find_last_of(end) == chain.size() - 1;
}
