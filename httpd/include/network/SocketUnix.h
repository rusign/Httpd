#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <Logger/Logger.hh>
#include "ISocket.h"

namespace zia
{
    class SocketUnix : public ISocket
    {
    public:
        SocketUnix(apouche::Logger const &, const std::string, uint16_t);
        SocketUnix(apouche::Logger const &, uint16_t, int = 0);
        SocketUnix(apouche::Logger const &, int, struct sockaddr_in);
        virtual ~SocketUnix();

        void connect();
        ISocket *accept() const;

        apouche::IZiaConnection::NativeHandle getNativeHandle() const;
        int ReceiveRequest();
        int SendResponse(apouche::IHttpResponse *);
        std::vector<apouche::IHttpRequest *> const &getRequests() const;
        apouche::IHttpRequest *getNextRequest();

        bool processing() const { return _processing; }
        bool disconnected() const { return _disconnected; }
        void process() { _processing = true; }
        void unprocess() { _processing = false; }
        void disconnect() { _disconnected  = true; }

    private:
        void bind();
        void listen(int);

    private:
        bool                                 _processing;
        bool                                 _disconnected;

        int                                  _csock;
        int                                  _optval;
        struct sockaddr_in                   _sin;
        apouche::Logger const                &_logger;
        std::vector<apouche::IHttpRequest *> _requests;
    };

    using Socket = SocketUnix;
}