#include <functional>
#include <api/Logger.hh>
#include "exception/http_exception.hh"
#include "exception/io_exception.hh"
#include "httpEngine/Engine.hh"
#include "httpEngine/Response.hh"
#include "utils/ForlderView.hh"

const zia::HttpResponse zia::httpEngine::Engine::operator()(const zia::HttpRequest &request, zia::Logger &logger) const
{
    auto response = zia::httpEngine::Response();
    auto host     = request.getHeaders()->getHeader("host");
    host = host.substr(0, host.find(':'));
    auto directoryPath = "sites:" + host + ":directory";

    std::string directory;
    if (conf_->containsConf(directoryPath))
        directory = conf_->getConf(directoryPath);
    else if (conf_->containsConf("sites:\"*\":directory"))
    {
        directory = conf_->getConf("sites:\"*\":directory");
        host = "\"*\"";
    }
    else
    {
        logger.error(std::string("[Server] Internal error: directory not set for " + host));
        return (response.generate(apouche::InternalServerError, nullptr));
    }

    try
    {
        switch (request.getMethod())
        {
            case apouche::Get:
                return Get(request, host, directory);
            case apouche::Head:
                return Head(request, host, directory);
            default:
                return zia::httpEngine::Response().generate(apouche::MethodNotAllowed, NotAllowed);
        }
    }
    catch (zia::http_exception &e)
    {
        logger.exception("Server", e);
        return e.getResponse();
    }
    catch (zia::custom_exception &e)
    {
        logger.exception("Server", e);
        return zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr);
    }
    catch (std::exception &e)
    {
        logger.error(std::string("[Server] Internal error: ") + e.what());
        return zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr);
    }
}

const zia::HttpResponse zia::httpEngine::Engine::readFromFile(const zia::HttpRequest &request,
                                                              const std::string &directory,
                                                              bool folderview) const TRACE_EXCEPTION
({
     auto response = zia::httpEngine::Response();

     try
     {
         auto reader  = zia::io::ioReader(directory + request.getURL(), mutex_);
         auto content = reader.readFromFile();
         return response.generate(apouche::OK, [&](HttpResponse &response__)
         {
             return Ok(response__, content);
         });
     }
     catch (zia::custom_exception &e)
     {
         std::string content;
         if (folderview)
             content = zia::utils::FolderView().view(directory, request.getURL());
         if (content.empty())
             throw zia::http_exception(e.what(),
                                       response.generate(apouche::NotFound, nullptr));
         else
             return response.generate(apouche::OK, [&](HttpResponse &response__)
             {
                 return Ok(response__, content);
             });
     }
     catch (std::exception &e)
     {
         throw zia::http_exception(std::string("Internal error: ") + e.what(),
                                   response.generate(apouche::InternalServerError, nullptr));
     }
 })

void zia::httpEngine::Engine::NotAllowed(zia::HttpResponse &response)
{
    response.getHeaders()->setHeader("allow", "GET, HEAD");
}

void zia::httpEngine::Engine::Ok(zia::HttpResponse &response, const std::string &content)
{
    response.getHeaders()->setHeader("content-length", std::to_string(content.size()));
    response.getBody()->setBody(content);
}