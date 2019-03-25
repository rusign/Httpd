#include <cstddef>
#include <limits>
#include <parser/YamlParser.hpp>
#include <utils/RequestExtractor.hh>
#include "api/HttpRequest.hpp"
#include "api/HttpResponse.hpp"
#include "network/SocketUnix.h"
#include "utils/Splitter.hh"

zia::SocketUnix::SocketUnix(apouche::Logger const &logger, const std::string ip, uint16_t port) :
        _csock(0),
        _logger(logger),
        _disconnected(false),
        _processing(false) TRACE_EXCEPTION
({
     _sin.sin_addr.s_addr = inet_addr(ip.c_str());
     _sin.sin_family      = AF_INET;
     _sin.sin_port        = htons(port);
     if ((_csock = socket(AF_INET, (SOCK_STREAM), 0)) == -1)
         throw zia::socket_exception("socket creation error");
     this->connect();
 })
zia::SocketUnix::SocketUnix(apouche::Logger const &logger, uint16_t port, int queue) :
        _logger(logger),
        _disconnected(false),
        _processing(false) TRACE_EXCEPTION
({
     _optval = 1;
     _sin.sin_addr.s_addr = htonl(INADDR_ANY);
     _sin.sin_family      = AF_INET;
     _sin.sin_port        = htons(port);
     if ((_csock = socket(AF_INET, (SOCK_STREAM), 0)) == -1)
         throw zia::socket_exception("socket creation error");
     if (setsockopt(_csock, SOL_SOCKET, SO_REUSEADDR, &_optval, sizeof(int)) == -1)
         throw zia::socket_exception("socket creation error");
     this->bind();
     this->listen(queue);
 })
zia::SocketUnix::SocketUnix(apouche::Logger const &logger, int sock, struct sockaddr_in sin) :
        _csock(sock),
        _sin(sin),
        _disconnected(false),
        _processing(false),
        _logger(logger) {}

zia::SocketUnix::~SocketUnix()
{
     close(_csock);
}

void zia::SocketUnix::connect() TRACE_EXCEPTION
({
     if (::connect(_csock, (struct sockaddr *) &(_sin), sizeof(_sin)) != 0)
         throw zia::socket_exception("connect error");
 })
zia::ISocket *zia::SocketUnix::accept() const TRACE_EXCEPTION
({
     ISocket            *clientSocket;
     int                newSock;
     struct sockaddr_in csin;
     unsigned int       csinSize = sizeof(csin);

     if ((newSock = ::accept(_csock, (struct sockaddr *) &csin, &csinSize)) == -1)
         throw zia::socket_exception("accept error");

     clientSocket = new SocketUnix(_logger, newSock, csin);
     return clientSocket;
 })

void zia::SocketUnix::bind() TRACE_EXCEPTION
({
     if (::bind(_csock, (struct sockaddr *) &(_sin), sizeof(_sin)) != 0)
         throw zia::socket_exception("bind error");
 })
void zia::SocketUnix::listen(int queue) TRACE_EXCEPTION
({
     if (::listen(_csock, queue) != 0)
         throw zia::socket_exception("listen error");
 })

apouche::IZiaConnection::NativeHandle zia::SocketUnix::getNativeHandle() const { return _csock; }

int zia::SocketUnix::ReceiveRequest() TRACE_EXCEPTION
({
     zia::utils::RequestExtractor extractor;
     std::string                  buffer;
     ::ssize_t                    _rd;
     char                         buff[2049];

     _requests.clear();
     buffer.clear();
     while ((_rd = read(_csock, buff, 2048)) > 0)
     {
         buff[_rd] = '\0';
         buffer += buff;
         if (_rd < 2048)
             break;
     }
     if (_rd < 0)
         return static_cast<int>(_rd);

     _requests = extractor(buffer);
     if (_requests.size() > std::numeric_limits<int>::max())
         return std::numeric_limits<int>::max();
     return static_cast<int>(_requests.size());
 })
int zia::SocketUnix::SendResponse(apouche::IHttpResponse *httpResponse) TRACE_EXCEPTION
({
     if (httpResponse == nullptr)
         return -1;

     auto response = zia::HttpResponse(httpResponse).serialize();
     if (response.empty())
         return -1;
     if (::send(_csock, response.c_str(), response.size(), 0) == -1)
         throw zia::socket_exception("send error");
     return 0;
 })
std::vector<apouche::IHttpRequest *> const &zia::SocketUnix::getRequests() const
{
    return _requests;
}
apouche::IHttpRequest *zia::SocketUnix::getNextRequest()
{
    if (_requests.empty())
        return nullptr;
    auto front = _requests.front();
    _requests.erase(_requests.begin());
    return front;
}