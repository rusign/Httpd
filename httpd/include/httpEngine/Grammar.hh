#pragma once

#include <functional>
#include <map>
#include <string>
#include <Enum.hh>
#include <HttpMessage/IHttpMessage.hh>

#define _(d)    + zia::httpEngine::Grammar::d +

namespace zia
{
    namespace httpEngine
    {
        using StatusCodeName = std::map<apouche::StatusCode, const std::string>;
        using MethodName = std::map<apouche::Method, const std::string>;

        template<typename TValue>
        using Dictionnary = std::map<const std::string, TValue>;
        template<int N>
        using Lexicon = std::array<const std::string, N>;

        struct Grammar
        {
            enum Type { Request, Response };
            static const std::string CR;
            static const std::string LF;
            static const std::string SP;
            static const std::string HT;
            static const std::string CRLF;

            struct Date
            {
                static const std::string rfc1123_date;
                static const std::string rfc850_date;
                static const std::string asciitime_date;
                static const std::string date1;
                static const std::string date2;
                static const std::string date3;
                static const std::string time;

                static const std::string weekday;
                static const std::string month;
            };

            static const StatusCodeName                                                  StatusCode;
            static const MethodName                                                      Method;
            static const Dictionnary<std::function<void(Type, apouche::IHttpMessage &)>> ContentEncoding;
        };
    }
}