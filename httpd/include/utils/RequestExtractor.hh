#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <HttpRequest/IHttpRequest.hh>

namespace zia
{
    namespace utils
    {
        using Requests = std::vector<apouche::IHttpRequest *>;

        class RequestExtractor
        {
            using MethodExtractor = std::map<apouche::Method, std::function<std::string(apouche::IHttpRequest *)>>;

        public:
            RequestExtractor();

            const Requests operator()(const std::string &);

        private:
            apouche::IHttpRequest *extractRequest(std::string &);

            std::string extractGet(apouche::IHttpRequest *);
            std::string extractPost(apouche::IHttpRequest *);
            std::string extractNone(apouche::IHttpRequest *);

            std::string extractWithSize(apouche::IHttpRequest *);
            std::string extractWithChunk(apouche::IHttpRequest *);

        private:
            MethodExtractor extractors_;
        };
    }
}