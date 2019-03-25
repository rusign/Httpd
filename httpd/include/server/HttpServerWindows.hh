#pragma once

#include <winsock2.h>
#include <list>
#include <signal.h>
#include "api/Logger.hh"
#include "network/ISocket.h"
#include "parser/YamlParser.hpp"
#include "server/IHttpServer.hh"
#include "worker/WorkerManager.hpp"

namespace zia
{
    class HttpServerWindows : public IHttpServer {
        using PollList = std::list<std::pair<WSAPOLLFD, ISocket *>>;

    public:
        HttpServerWindows(zia::Logger &, std::mutex &);

        bool run(const std::string &);

    private:
        uint16_t port() const;

        bool confUpdated(const std::string &) const;
        bool newClient();

    	bool isOnline(const pollfd &, const ISocket *) const;
        bool deleteClient(int i);
        void newTask(ISocket *);
        void destroyServer();


        bool _serverIsAlive;
		WSAPOLLFD _serverFd;
        zia::WorkerManager _wm;
        zia::Logger &_logger;
        PollList _pollList;
        YamlParser &_conf;
    };
    using HttpServer = HttpServerWindows;
}