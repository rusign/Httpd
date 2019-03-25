#include <algorithm>
#include <httpEngine/Response.hh>
#include "server/HttpServerUnix.hh"

static volatile bool keepRunning = true;

void intHandler(int dummy)
{
    keepRunning = false;
}

zia::HttpServerUnix::HttpServerUnix(zia::Logger &logger, std::mutex &guard) :
        _wm(std::make_unique<zia::SocketQueue>(), guard, logger),
        _logger(logger),
        _conf(zia::YamlParser::getInstance())
{
    auto port_  = port();
    auto socket = new Socket(_logger, port_);

    _logger.info("[Server] Start listening on " + std::to_string(port_));

    _serverFd.fd     = socket->getNativeHandle();
    _serverFd.events = POLLIN;
    _serverIsAlive = true;
    _pollList.push_back(std::make_pair(_serverFd, socket));

    signal(SIGINT, intHandler);
}

uint16_t zia::HttpServerUnix::port()
{
    auto conf = _conf.getHttpConf();
    if (!conf.containsConf("server:port"))
    {
        _logger.info("[Configuration] HTTP Server port not set");
        return DEFAULT_PORT;
    }

    uint64_t port;
    try
    {
        port = std::stoul(conf.getConf("server:port"));
    }
    catch (...)
    {
        _logger.info("[Configuration] Invalid HTTP Server port");
        return DEFAULT_PORT;
    }

    if (port > std::numeric_limits<uint16_t>::max())
    {
        _logger.info("[Configuration] Invalid HTTP Server port");
        return DEFAULT_PORT;
    }
    return static_cast<uint16_t>(port);
}

bool zia::HttpServerUnix::run(const std::string &filename)
{
    PollList::iterator it;
    int                i;
    bool               isrun = false;

    while (_serverIsAlive && keepRunning)
    {
        if (confUpdated(filename) && (isrun = true))
            break;

        pollfd fds_array[_pollList.size()];
        for (i = 0, it = _pollList.begin(); it != _pollList.end(); i++, it++)
            fds_array[i] = it->first;

        if (poll(fds_array, (nfds_t) _pollList.size(), 2500) < 0)
            break;

        if (fds_array[0].revents & POLLIN && newClient())
            continue;

        for (i = 1; i < _pollList.size(); ++i)
        {
            PollList::iterator ptr;
            if ((ptr = std::next(_pollList.begin(), i)) == _pollList.end())
                continue;
            auto socket = ptr->second;

            if (!isOnline(fds_array[i], socket))
                deleteClient(i);
            else if (fds_array[i].revents & POLLIN && !socket->processing())
                newTask(socket);
        }
    }
    destroyServer();
    return isrun;
}

bool zia::HttpServerUnix::confUpdated(const std::string &filename) const
{
    struct stat result;
    return stat(filename.c_str(), &result) == 0 && _conf.getpreviousTime() != result.st_mtime;
}

bool zia::HttpServerUnix::newClient()
{
    ISocket *clientSock;
    pollfd  new_fd;

    clientSock = _pollList.front().second->accept();
    _logger.info("[Client] New connexion (Socket #" + std::to_string(clientSock->getNativeHandle()) + ")");

    new_fd.fd     = clientSock->getNativeHandle();
    new_fd.events = POLLIN;
    _pollList.push_back(std::make_pair(new_fd, clientSock));
    return true;
}

bool zia::HttpServerUnix::isOnline(const pollfd &poll, const ISocket *socket) const
{
    char c;
    return socket->processing() ||
           (::recv(socket->getNativeHandle(), &c, 1, MSG_PEEK | MSG_DONTWAIT) != 0 && !socket->disconnected());
}

bool zia::HttpServerUnix::deleteClient(int index)
{
    auto it = std::next(_pollList.begin(), index);
    _logger.info("[Client] Disconnected (Socket #" + std::to_string(it->second->getNativeHandle()) + ")");
    delete it->second;
    _pollList.erase(it);
    return true;
}

void zia::HttpServerUnix::newTask(ISocket *socket)
{
    socket->process();
    _wm.delegateTask(socket);
}

void zia::HttpServerUnix::destroyServer()
{
    _wm.killWorkers();
    for (auto socket : _pollList)
        delete socket.second;
    _pollList.clear();
}

