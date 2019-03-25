#pragma once

#include <mutex>
#include <poll.h>
#include <signal.h>
#include "api/Logger.hh"
#include "network/ISocket.h"
#include "parser/YamlParser.hpp"
#include "server/IHttpServer.hh"
#include "worker/WorkerManager.hpp"

namespace zia
{
    class HttpServerUnix : public IHttpServer
    {
        using PollList = std::list<std::pair<pollfd, ISocket *>>;

    public:
        HttpServerUnix(zia::Logger &, std::mutex &);
        bool run(const std::string &);

    private:
        uint16_t port();
        bool confUpdated(const std::string &) const;

        bool newClient();
        bool isOnline(const pollfd &, const ISocket *) const;
        bool deleteClient(int i);

        void newTask(ISocket *);

        void destroyServer();


        bool               _serverIsAlive;
        pollfd             _serverFd;
        zia::WorkerManager _wm;
        apouche::Logger    &_logger;
        PollList           _pollList;
        YamlParser         &_conf;
    };

    using HttpServer = HttpServerUnix;
}
