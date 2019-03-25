#include "exception/custom_exception.hh"
#include "httpEngine/Response.hh"
#include "httpEngine/Engine.hh"

const zia::HttpResponse zia::httpEngine::Engine::Head(const zia::HttpRequest &request,
                                                      const std::string &host,
                                                      const std::string &directory) const TRACE_EXCEPTION
({
     auto response = Get(request, host, directory);
     response.getBody()->setBody("");
     response.getHeaders()->setHeader("content-length", "0");
     response.getHeaders()->deleteHeader("transfert-encoding");

     return response;
 })