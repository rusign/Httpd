#define _WINSOCK_DEPRECATED_NO_WARNINGS
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#include <limits>
#include <Logger/Logger.hh>
#include "api/HttpResponse.hpp"
#include "network/SocketWindows.h"
#include "utils/RequestExtractor.hh"



zia::SocketWindows::SocketWindows(apouche::Logger const &logger, const std::string ip, uint16_t port) :
	_csock(0),
                                                                                                  _logger(logger),
                                                                                                  _disconnected(false),
                                                                                                  _processing(false)
{
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    _sin.sin_addr.s_addr = inet_addr(ip.c_str());
    _sin.sin_family      = AF_INET;
    _sin.sin_port        = htons(port);
    if ((_csock = socket(AF_INET, (SOCK_STREAM), 0)) == INVALID_SOCKET)
        throw zia::socket_exception("socket creation error");
    this->connect();
}

zia::SocketWindows::SocketWindows(apouche::Logger const &logger, uint16_t port, int queue) : 
	_logger(logger),
_disconnected(false),
                                                                                       _processing(false)
{
    _optval = 1;
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    _sin.sin_addr.s_addr = INADDR_ANY;
    _sin.sin_family      = AF_INET;
    _sin.sin_port        = htons(port);
    if ((_csock = socket(AF_INET, (SOCK_STREAM), 0)) == -1)
        throw zia::socket_exception("socket creation error");
    if (setsockopt(_csock, SOL_SOCKET, SO_REUSEADDR, (char *)&_optval, sizeof(int)) == -1)
        throw zia::socket_exception("socket creation error");
    this->bind();
    this->listen(queue);
}
zia::SocketWindows::SocketWindows(apouche::Logger const &logger, SOCKET sock, SOCKADDR_IN sin) : 
	_csock(sock),
                                                                                               _sin(sin),
                                                                                               _disconnected(false),
                                                                                               _processing(false),
                                                                                               _logger(logger) {}

void zia::SocketWindows::bind()
{
    if (::bind(_csock, (SOCKADDR * ) & (_sin), sizeof(_sin)) != 0)
        throw zia::socket_exception("bind error");
}

void zia::SocketWindows::listen(int queue)
{
    if (::listen(_csock, queue) != 0)
        throw zia::socket_exception("listen error");
}

void zia::SocketWindows::connect()
{
    if (::connect(_csock, (SOCKADDR * ) & (_sin), sizeof(_sin)) != 0)
        throw zia::socket_exception("connect error");
}
zia::ISocket *zia::SocketWindows::accept() const
{
    ISocket            *clientSocket;
    SOCKET                newSock;
    SOCKADDR_IN         csin;
    int       csinSize = sizeof(csin);

    if ((newSock = ::accept(_csock, (SOCKADDR * ) & csin, &csinSize)) == INVALID_SOCKET)
        throw zia::socket_exception("accept error");
    u_long iMode=1;
    ioctlsocket(newSock,FIONBIO,&iMode);
    clientSocket = new SocketWindows(_logger, newSock, csin);
    return clientSocket;
}
apouche::IZiaConnection::NativeHandle zia::SocketWindows::getNativeHandle() const { return _csock; }

int zia::SocketWindows::ReceiveRequest()
{
    zia::utils::RequestExtractor extractor;
    std::string          buffer;
    ::ssize_t            _rd;
    char                 buff[2049];

    _requests.clear();
    buffer.clear();
    while ((_rd = ::recv(_csock, buff, 2048,0)) > 0)
    {
        buff[_rd] = '\0';
        buffer += buff;
        if (_rd < 2048)
            break;
    }
    if (_rd < 0)
        return static_cast<int>(_rd);

    _requests = extractor(buffer);
    if (_requests.size() > INT_MAX)
        return INT_MAX;
    return static_cast<int>(_requests.size());
}

int zia::SocketWindows::SendResponse(apouche::IHttpResponse *httpResponse)
{
    if (httpResponse == nullptr)
        return -1;

    auto response = zia::HttpResponse(httpResponse).serialize();
    if (response.empty())
        return -1;
    if (::send(_csock, response.c_str(), response.size(), 0) == -1)
        throw zia::socket_exception("send error");
}

std::vector<apouche::IHttpRequest *> const &zia::SocketWindows::getRequests() const
{
    return _requests;
}
apouche::IHttpRequest *zia::SocketWindows::getNextRequest()
{
    if (_requests.empty())
        return nullptr;
    auto front = _requests.front();
    _requests.erase(_requests.begin());
    return front;
}

zia::SocketWindows::~SocketWindows()
{
	::closesocket(this->_csock);
}
