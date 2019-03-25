#pragma once
#pragma comment(lib, "wsock32.lib")

#include <iostream>
#include <string>
//#include <winsock2.h>
#include <windows.h>
#include <Logger/Logger.hh>
#include "ISocket.h"

namespace zia
{
    class SocketWindows : public ISocket
    {
    public:
        SocketWindows(apouche::Logger const &, const std::string, uint16_t);
        SocketWindows(apouche::Logger const &, uint16_t, int = 0);
        SocketWindows(apouche::Logger const &, SOCKET, SOCKADDR_IN);
        virtual ~SocketWindows();

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
		void disconnect() { _disconnected = true; }

    private:
        void bind();
        void listen(int);

    private:
        bool                                 _processing;
        bool                                 _disconnected;

        SOCKET                               _csock;
        int                                  _optval;
        SOCKADDR_IN                          _sin;
        apouche::Logger const                &_logger;
        std::vector<apouche::IHttpRequest *> _requests;
    };


    using Socket = SocketWindows;
}
