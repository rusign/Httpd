#pragma once

#include <functional>
#include "api/HttpResponse.hpp"

namespace zia
{
    namespace httpEngine
    {
        class Response
        {
        public:
            bool validate(const apouche::IHttpResponse *);
            zia::HttpResponse generate(apouche::StatusCode, const std::function<void(zia::HttpResponse &)> &) const;
        };
    }
}