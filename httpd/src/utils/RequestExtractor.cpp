#include <algorithm>
#include <functional>
#include <iostream>

#include "api/HttpRequest.hpp"
#include "exception/splitter_exception.hh"
#include "httpEngine/Grammar.hh"
#include "utils/RequestExtractor.hh"
#include "utils/Splitter.hh"

namespace zia
{
    namespace utils
    {
        RequestExtractor::RequestExtractor()
        {
            extractors_ = {
                    {apouche::Method::Options, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)},
                    {apouche::Method::Get, std::bind(&RequestExtractor::extractGet, this, std::placeholders::_1)},
                    {apouche::Method::Head, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)},
                    {apouche::Method::Post, std::bind(&RequestExtractor::extractPost, this, std::placeholders::_1)},
                    {apouche::Method::Put, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)},
                    {apouche::Method::Delete, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)},
                    {apouche::Method::Trace, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)},
                    {apouche::Method::Connect, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)},
                    {apouche::Method::UndefinedRequestMethod, std::bind(&RequestExtractor::extractNone, this, std::placeholders::_1)}
            };
        }

        const Requests RequestExtractor::operator()(const std::string &requests)
        {
            std::string           requests_cpy(requests);
            apouche::IHttpRequest *request;
            zia::utils::Requests  requests_;

            for (;;)
            {
                request = extractRequest(requests_cpy);
                if (!request)
                    break;
                requests_.push_back(request);
            }

            return requests_;
        }

        apouche::IHttpRequest *RequestExtractor::extractRequest(std::string &requests)
        {
            if (requests.empty())
                return nullptr;

            auto request = new zia::HttpRequest(requests);
            requests = extractors_[request->getMethod()](request);
            return request;
        }

        std::string RequestExtractor::extractGet(apouche::IHttpRequest *request)
        {
            auto rest = request->getBody()->getBody();
            request->getBody()->setBody("");
            return rest;
        }

        std::string RequestExtractor::extractPost(apouche::IHttpRequest *request)
        {
            bool hasCL = request->getHeaders()->containsHeader("content-length");
            bool hasTE = request->getHeaders()->containsHeader("transfert-encoding") &&
                         request->getHeaders()->getHeader("transfert-encoding") == "chunked";

            if (hasTE)
                return extractWithChunk(request);
            else if (hasCL)
                return extractWithSize(request);
            return "";
        }
        std::string RequestExtractor::extractNone(apouche::IHttpRequest *) { return ""; }

        std::string RequestExtractor::extractWithSize(apouche::IHttpRequest *request)
        {
            uint64_t size = std::stoull(request->getHeaders()->getHeader("content-length"));

            auto rest = request->getBody()->getBody().substr(size);
            request->getBody()->setBody(request->getBody()->getBody().substr(0, size - 1));

            return rest;
        }

        std::string RequestExtractor::extractWithChunk(apouche::IHttpRequest *request)
        {
            Splitter splitter;
            std::string content = "";

            auto rest = request->getBody()->getBody();
            for (;;)
            {
                auto frag_length = std::stoull(splitter.split(rest, zia::httpEngine::Grammar::CRLF));
                if (!frag_length)
                {
                    rest = rest.substr(2);
                    break;
                }
                auto frag_content = rest.substr(0, frag_length - 1);
                rest = rest.substr(frag_length + 2);
                content += frag_content;
            }
            request->getBody()->setBody(content);
            request->getHeaders()->setHeader("content-length", std::to_string(content.size()));
            request->getHeaders()->setHeader("transfert-encoding", "identity");

            return rest;
        }
    }
}