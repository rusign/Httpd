#include <iostream>
#include <mutex>
#include "api/HttpRequest.hpp"
#include "exception/http_exception.hh"
#include "httpEngine/Engine.hh"
#include "httpEngine/Response.hh"
#include "parser/YamlParser.hpp"
#include "worker/Worker.hh"

void zia::Worker::treat(ISocket *socket, std::mutex *mutex, zia::Logger *logger)
{
    auto conf   = zia::YamlParser::getInstance().getHttpConf();
    auto events = zia::YamlParser::getInstance().getHttpEvents();
    auto engine = zia::httpEngine::Engine(*mutex, &conf);

    events._beforeParsingRequest.callAllEvent(&conf);
    try
    {
        socket->ReceiveRequest();
    }
    catch (zia::http_exception &e)
    {
        logger->exception("Client", e);
        auto response = e.getResponse();
        socket->SendResponse(response);
    }
    catch (std::exception &e) { logger->error(std::string("[Client] Unexpected exception: ") + e.what()); }

    try
    {
        apouche::IHttpRequest *request_ = nullptr;
        while ((request_ = socket->getNextRequest()) != nullptr)
        {
            try
            {
                zia::HttpRequest request(request_);
                logger->info("[Client] New request: " + request.getRequestLine());
                if (request.getHeaders()->containsHeader("connection") &&
                    request.getHeaders()->getHeader("connection") == "close")
                    socket->disconnect();

                events._afterParsingRequest.callAllEvent(&request, &conf);
                events._requestReceived.callAllEvent(&request, &conf, socket);

                events._beforeCreateResponse.callAllEvent(&request, &conf);
                auto response = engine(request, *logger);
                events._afterCreateResponse.callAllEvent(&request, &response, &conf);

                if (response.getHeaders()->containsHeader("connection") &&
                    response.getHeaders()->getHeader("connection") == "close")
                    socket->disconnect();
                events._beforeSendResponse.callAllEvent(&response, &conf);
                socket->SendResponse(&response);
                events._afterSendResponse.callAllEvent(socket);
            }
            catch (zia::http_exception &e)
            {
                logger->exception("Server", e);
                auto response = e.getResponse();
                socket->SendResponse(response);
            }
            catch (std::exception &e)
            {
                logger->error(std::string("[Server] Unexpected exception: ") + e.what());
                auto response = zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr);
                socket->SendResponse(&response);
            }
            catch (std::exception *e)
            {
                logger->error(std::string("[Server] Unexpected exception: ") + e->what());
                auto response = zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr);
                socket->SendResponse(&response);
            }
            catch (...)
            {
                auto response = zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr);
                socket->SendResponse(&response);
            }

            delete request_;
        }
    }
    catch (zia::http_exception &e)
    {
        logger->exception("Server", e);
        auto response = e.getResponse();
        socket->SendResponse(response);
    }
    catch (std::exception &e) { logger->error(std::string("[Server] Unexpected exception: ") + e.what()); }
    catch (std::exception *e) { logger->error(std::string("[Server] Unexpected exception: ") + e->what()); }
    catch (...) { logger->error("[Server] Unexpected exception"); }
    socket->unprocess();
}