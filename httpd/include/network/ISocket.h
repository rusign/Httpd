#pragma once

#include <Network/IZiaConnection.hpp>
#include "exception/socket_exception.hh"

namespace zia
{
    class ISocket : public apouche::IZiaConnection
    {
    public:
        virtual ~ISocket() {};

        virtual void connect() = 0;
        virtual ISocket *accept() const = 0;

        apouche::IZiaConnection::NativeHandle getNativeHandle() const = 0;
        int ReceiveRequest() = 0;
        int SendResponse(apouche::IHttpResponse *) = 0;

        std::vector<apouche::IHttpRequest *> const &getRequests() const = 0;
        apouche::IHttpRequest *getNextRequest() = 0;

        virtual bool processing() const = 0;
        virtual bool disconnected() const = 0;
        virtual void process() = 0;
        virtual void unprocess() = 0;
        virtual void disconnect() = 0;

    private:
        virtual void bind() = 0;
        virtual void listen(int) = 0;
    };
}

#ifdef _WIN32
    #include "network/SocketWindows.h"
#else
    #include "network/SocketUnix.h"
#endif