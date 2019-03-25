#pragma once

#include <HttpConf/IHttpConf.hpp>
#include "api/HttpRequest.hpp"
#include "api/HttpResponse.hpp"
#include "api/Logger.hh"
#include "httpEngine/Grammar.hh"
#include "io/ioReader.hh"

#define SERVER_NAME "Azia"
#define SERVER_VERSION "1.0.0"

namespace zia
{
    namespace httpEngine
    {
        class Engine
        {
        public:
            Engine(std::mutex &m, const apouche::IHttpConf *c) : mutex_(m), conf_(c) {}
            const zia::HttpResponse operator()(const zia::HttpRequest &, zia::Logger &) const;

        private:
            const zia::HttpResponse Get(const zia::HttpRequest &, const std::string &, const std::string &) const;
            const zia::HttpResponse Head(const zia::HttpRequest &, const std::string &, const std::string &) const;

        private:
            const zia::HttpResponse readFromFile(const zia::HttpRequest &, const std::string &, bool) const;

        private:
            static void NotAllowed(zia::HttpResponse &);
            static void Ok(zia::HttpResponse &, const std::string &);

        private:
            std::mutex               &mutex_;
            const apouche::IHttpConf *conf_;
        };
    }
}