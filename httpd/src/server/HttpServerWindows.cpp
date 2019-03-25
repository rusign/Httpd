#pragma comment(lib, "ws2_32.lib")

#include <sstream>
#include "server/HttpServerWindows.hh"

static volatile bool keepRunning = true;

void intHandler(int dummy)
{
    keepRunning = false;
}

zia::HttpServerWindows::HttpServerWindows(zia::Logger &logger, std::mutex &guard) : 
	_wm(std::make_unique<zia::SocketQueue>(), guard, logger),
                                                                                    _logger(logger),
                                                                                    _conf(zia::YamlParser::getInstance())
{
    auto port_  = port();
    auto socket = new Socket(_logger, port_);

    _logger.info("Start listening on " + std::to_string(port_));

    _serverFd.fd     = socket->getNativeHandle();
    _serverFd.events = POLLIN;
    _serverIsAlive = true;
    _pollList.push_back(std::make_pair(_serverFd, socket));

    signal(SIGINT, intHandler);
}

uint16_t zia::HttpServerWindows::port() const
{
    auto conf = _conf.getHttpConf();
    if (!conf.containsConf("server:port"))
        return DEFAULT_PORT;

    uint64_t port;
    try
    {
        port = std::stoul(conf.getConf("server:port"));
    }
    catch (...) { return DEFAULT_PORT; }

    if (port > USHRT_MAX)
        return DEFAULT_PORT;
    return static_cast<uint16_t>(port);
}

bool zia::HttpServerWindows::run(const std::string &filename)
{
    PollList::iterator it;
	WSAPOLLFD *fds_array = nullptr;
	int                i;
    bool               isrun = false;

    while (_serverIsAlive && keepRunning)
    {
        if (confUpdated(filename) && (isrun = true))
            break;

		if (fds_array)
			delete fds_array;
		fds_array = new WSAPOLLFD[_pollList.size()];

		for (i = 0, it = _pollList.begin(); it != _pollList.end(); i++, it++)
            fds_array[i] = it->first;

        if (WSAPoll(fds_array, (ULONG) _pollList.size(), 5000) < 0)
            break;

        if (fds_array[0].revents & POLLIN && newClient())
            continue;

        for (i = 1; i < _pollList.size(); ++i)
        {
            PollList::iterator ptr;
            if ((ptr = std::next(_pollList.begin(), i)) == _pollList.end())
                continue;
            auto socket = ptr->second;

            if (!isOnline(fds_array[i], socket)) {
                deleteClient(i);
                break;
            }
            else if (fds_array[i].revents & POLLIN && !socket->processing())
                newTask(socket);
        }

    }
	if (fds_array)
		delete fds_array;
    destroyServer();
	WSACleanup();
	return isrun;
}

bool zia::HttpServerWindows::confUpdated(const std::string &filename) const
{
    struct stat result;
    return stat(filename.c_str(), &result) == 0 && _conf.getpreviousTime() != result.st_mtime;
}

bool zia::HttpServerWindows::newClient()
{
    ISocket *clientSock;
    pollfd  new_fd;

    clientSock = _pollList.front().second->accept();
    _logger.info("New client (Socket #" + std::to_string(clientSock->getNativeHandle()) + ")");

    new_fd.fd     = clientSock->getNativeHandle();
    new_fd.events = POLLIN;
    _pollList.push_back(std::make_pair(new_fd, clientSock));
    return true;
}

bool zia::HttpServerWindows::isOnline(const pollfd &poll, const ISocket *socket) const
{
    char c;
    return socket->processing() || ::recv(socket->getNativeHandle(), &c, 1, MSG_PEEK /* MSG_DONTWAIT*/) != 0;
}

bool zia::HttpServerWindows::deleteClient(int index)
{
    auto it = std::next(_pollList.begin(), index);
    _logger.info("Client disconnected (Socket #" + std::to_string(it->second->getNativeHandle()) + ")");
    //delete it->second;
    _pollList.erase(it);
    return true;
}

void zia::HttpServerWindows::newTask(ISocket *socket)
{
    socket->process();
    _wm.delegateTask(socket);
}

void zia::HttpServerWindows::destroyServer()
{
    _wm.killWorkers();
    for (auto socket : _pollList)
        delete socket.second;
    _pollList.clear();
}
