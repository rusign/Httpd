#include <cerrno>
#include <cstring>
#include "server/IHttpServer.hh"
#include "api/FileLogger.hh"
#include "httpd.hh"

#ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0501
#endif

#ifndef WINVER
#  define WINVER _WIN32_WINN
#endif

int main(int argc, const char *argv[])
{
    bool        help     = false;
    bool        cout     = false;
    std::string confFile = "zia.yml";

    for (int i = 1; i < argc; i++)
        if (std::string(argv[i]) == "-h")
            help = true;
        else if (std::string(argv[i]) == "-")
            cout = true;
        else
            confFile = argv[i];

    if (!help)
        return zia::httpd::launch(confFile, cout);
    std::cout << argv[0] << " confFile [-h|-]" << std::endl;
    return 0;
}

int zia::httpd::launch(const std::string &confFile, bool toCout)
{
    std::mutex guard;

    try
    {
        bool        isRunning;
        zia::Logger logger = zia::Logger(true);

        zia::YamlParser::getInstance().setLogger(logger);
		if (!(isRunning = zia::YamlParser::getInstance().loadFile(confFile)))
			return 0x1;

        zia::YamlParser::getInstance().loadFile(confFile);
        while (isRunning)
        {

            auto conf         = zia::YamlParser::getInstance().getHttpConf();
            auto logFile      = conf.getConf("server:log:file");
            auto logStactrace = conf.containsConf("server:log:stacktrace") &&
                                conf.getConf("server:log:stacktrace") == "true";
            logger = toCout ? zia::Logger(logStactrace) : zia::FileLogger(logFile, logStactrace);
            zia::YamlParser::getInstance().setLogger(logger);

            try
            {
                zia::HttpServer server(logger, guard);
                isRunning = server.run(confFile);
                zia::YamlParser::getInstance().saveConf();
				if (isRunning && !zia::YamlParser::getInstance().loadFile(confFile))
					throw std::runtime_error("Invalid file");
            }
			catch (std::exception &e)
			{
				logger.warning(std::string("[Server] Server crashed, try to restore previous configuration (") +
					e.what() + ")");
				if (!zia::YamlParser::getInstance().restorePrevious())
				{
					logger.error("Failed to restore previous configuration, shutdown server");
					throw;
				}
			}
			catch (std::exception *e)
			{
				logger.warning(std::string("[Server] Server crashed, try to restore previous configuration (") +
					e->what() + ")");
				if (!zia::YamlParser::getInstance().restorePrevious())
				{
					logger.error("Failed to restore previous configuration, shutdown server");
					throw;
				}
			}
		}
    }
    catch (std::exception &e)
    {
        auto errno_ = errno;
        std::cerr << "[" << zia::utils::DateTime().currentTime() << "] "
                  << "[ FATAL ] [Server] System error: " << e.what();
        if (errno_)
            std::cerr << " { " << strerror(errno) << " (#" << errno << ") }";
        std::cerr << std::endl;
        return errno_;
    }

    return 0x0;
}