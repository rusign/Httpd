#pragma once

#include <vector>
#include <HttpRequest/HttpRequest.hh>

namespace zia
{
    namespace httpEngine
    {
        class Request
        {
        public:
            bool validate(const apouche::IHttpRequest *);
        };
    }
}