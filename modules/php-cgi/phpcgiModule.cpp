#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <EventHandler/EventHandler.hpp>
#include "utils/Uri.hh"
#include "phpcgiModule.hpp"
#include "utils/Trimmer.hh"

#define CHUNK           8192
#define BODY_DELIMTER   "\r\n\r\n"

namespace apouche
{
    void phpcgiModule::registerEvents(apouche::EventHandler *_handler)
    {
        _logger->info(_name + " v" + _version + ": Event registering -> PHP file interpretation");
        FunctionCGI function = [&](IHttpRequest *request, IHttpResponse *response, IHttpConf *conf)
        {
            this->cgiRequest(request, response, conf);
        };
        EventCGI    event("Response Rewriting", Weight::HIGH, function);

        _handler->_afterCreateResponse.addEvent(event);
    };

    std::string phpcgiModule::getConf(const std::string &path,
                                      const std::string &host,
                                      IHttpConf *conf)
    {
        std::string site_path    = "sites:" + host + path;
        std::string default_path = "sites:\"*\"" + path;

        if (conf->containsConf(site_path))
            return conf->getConf(site_path);
        else if (conf->containsConf(default_path))
            return conf->getConf(default_path);
        return "";
    }

    void phpcgiModule::cgiRequest(IHttpRequest *request, IHttpResponse *response, IHttpConf *conf)
    {
        int         pid;
        int         inputCGI[2];
        char        phpBuffer[CHUNK];
        char        **env = new char *[request->getHeaders()->getAllHeader().size() + 5];
        std::string cgi_output;


        std::string host   = request->getHeaders()->getHeader("host");
        auto        binary = getConf(":php-cgi:cgi-path", host, conf);
        auto        file   = getConf(":directory", host, conf) + utils::Uri().extractUrl(request->getURI());

        if (binary.empty())
            return;

        int  i       = 3;
        char *args[] = {strdup(binary.c_str()), NULL};

        env[0] = strdup("REDIRECT_STATUS=CGI");
        env[1] = strdup(("SCRIPT_FILENAME=" + file).c_str());
        env[2] = strdup(("REQUEST_METHOD=" + methodName_[request->getMethod()]).c_str());

        for (auto header : request->getHeaders()->getAllHeader())
            env[++i] = strdup((header.first + "=" + header.second).c_str());

        env[i] = NULL;
        pipe(inputCGI);

        if ((pid = fork()) == 0)
        {
            close(inputCGI[0]);
            dup2(inputCGI[1], 1);
            execve(args[0], args, env);
            throw std::runtime_error("An error occured using PHP-CGI binary !");
        }
        else
        {
            close(inputCGI[1]);
            cgi_output.clear();
            memset(phpBuffer, '\0', CHUNK);
            write(inputCGI[1], request->getBody()->getBody().c_str(), request->getBody()->getBody().size());
            while (read(inputCGI[0], phpBuffer, CHUNK) > 1)
            {
                cgi_output += phpBuffer;
                memset(phpBuffer, '\0', CHUNK);
            }
            setCGIheader(cgi_output, response);
        }

        response->getBody()->setBody(getCGIbody(cgi_output));
        response->getHeaders()->deleteHeader("Content-Length");
        response->getHeaders()->setHeader("content-length", std::to_string(response->getBody()->getBody().size()));
    }

    void phpcgiModule::setCGIheader(const std::string &cgi_output, IHttpResponse *response)
    {
        utils::Trimmer                     trimmer;
        std::size_t                        index = cgi_output.find(BODY_DELIMTER);
        std::map<std::string, std::string> header;
        std::size_t                        index_header;

        if (index != std::string::npos)
        {
            std::istringstream header_part(cgi_output.substr(0, index));
            std::string        elem;

            for (std::string line; getline(header_part, line);)
                if ((index = header_part.str().find(":")) != std::string::npos)
                {
                    auto key   = line.substr(0, index);
                    auto value = line.substr(index + 1);
                    response->getHeaders()->setHeader(trimmer.trim(key), trimmer.trim(value));
                }
        }

        if (!response->getHeaders()->containsHeader("Status"))
            return;
        auto status = response->getHeaders()->getHeader("Status");
        int  statusCode;
        try { statusCode = std::stoi(status); }
        catch (...) { throw std::runtime_error("Invalid status (PHP-CGI)"); }
        if (std::find(statusList_.begin(), statusList_.end(), statusCode) == statusList_.end() ||
            statusCode == apouche::UndefinedStatusCode)
            throw std::runtime_error("Invalid status (PHP-CGI)");
        response->setStatus(static_cast<apouche::StatusCode>(statusCode));
    };

    std::string phpcgiModule::getCGIbody(const std::string &cgi_output)
    {
        std::size_t index = cgi_output.find(BODY_DELIMTER);

        if (index != std::string::npos)
            return cgi_output.substr(index + std::string(BODY_DELIMTER).size());
        return "";
    };

    extern "C" AModule *instantiate() { return new apouche::phpcgiModule(); }
    extern "C" AModule *instantiateWithLogger(apouche::Logger *log) { return new apouche::phpcgiModule(log); }
}