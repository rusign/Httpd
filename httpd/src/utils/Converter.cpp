#ifdef _WIN32
	#pragma warning(disable : 4996)
#endif

#include <algorithm>
#include <ctime>
#include <Enum.hh>
#include <exception/custom_exception.hh>
#include "httpEngine/Grammar.hh"
#include "utils/Converter.hh"

const apouche::StatusCode zia::utils::Converter::toStatusCode(const std::string &statusCode) const
{
    try
    {
        auto status = static_cast<apouche::StatusCode>(std::stoll(statusCode));
        if (zia::httpEngine::Grammar::StatusCode.find(status) == zia::httpEngine::Grammar::StatusCode.end())
            return apouche::StatusCode::UndefinedStatusCode;
        return status;
    }
    catch (...)
    {
        return apouche::StatusCode::UndefinedStatusCode;
    }
}
const apouche::Method zia::utils::Converter::toMethod(const std::string &statusCode) const
{
    auto &&it = std::find_if(zia::httpEngine::Grammar::Method.begin(), zia::httpEngine::Grammar::Method.end(),
                             [&statusCode](const std::pair<const apouche::Method, const std::string> p)
                             {
                                 return p.second == statusCode;
                             });
    if (it == zia::httpEngine::Grammar::Method.end())
        return apouche::UndefinedRequestMethod;
    return it->first;
}
const uint64_t zia::utils::Converter::toDate(const std::string &date) const
{
    std::tm     time = {};
    std::time_t epoch;
    std::string format;

    if (date.size() < 5)
        return std::numeric_limits<uint64_t>::max();
    if (date[3] == ',')
        format = zia::httpEngine::Grammar::Date::rfc1123_date;
    else if (date[3] == ' ')
        format = zia::httpEngine::Grammar::Date::asciitime_date;
    else
        format = zia::httpEngine::Grammar::Date::rfc850_date;

    //TODO: Throw exception
    sscanf(date.c_str(), format.c_str(), &time.tm_year,&time.tm_mon, &time.tm_mday);
    if (/*!(strptime(date.c_str(), format.c_str(), &time)) ||*/ sscanf(date.c_str(), format.c_str(), &time.tm_year,&time.tm_mon, &time.tm_mday) || (epoch = std::mktime(&time)) < 0)
        throw zia::custom_exception("Invalid date format");
    return static_cast<uint64_t>(epoch);
}

const std::string &zia::utils::Converter::toString(apouche::Method method) const
{
    return zia::httpEngine::Grammar::Method.find(method)->second;
}
const std::string &zia::utils::Converter::toString(apouche::StatusCode status) const
{
    return zia::httpEngine::Grammar::StatusCode.find(status)->second;
}