#include <regex>
#include <HttpHeader/HttpHeader.hh>
#include <HttpBody/HttpBody.hh>
#include "exception/http_exception.hh"
#include "httpEngine/Grammar.hh"
#include "httpEngine/Request.hh"
#include "httpEngine/Response.hh"

bool zia::httpEngine::Request::validate(const apouche::IHttpRequest *request) TRACE_EXCEPTION
({
     if (request == nullptr || request->getHeaders() == nullptr || request->getBody() == nullptr)
         throw zia::http_exception("Invalid request instance",
                                       zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));

     if (request->getMethod() == apouche::Method::UndefinedRequestMethod)
         throw zia::http_exception("Undefined request method",
                                       zia::httpEngine::Response().generate(apouche::BadRequest, nullptr));

     std::regex version("HTTP/\\d+.\\d+");
     if (!std::regex_match(request->getVersion(), version))
         throw zia::http_exception("Invalid HTTP request version",
                                       zia::httpEngine::Response().generate(apouche::BadRequest, nullptr));
     if (request->getVersion() != "HTTP/1.1" && request->getVersion() != "HTTP/1.0")
         throw zia::http_exception("HTTP request version not supported",
                                       zia::httpEngine::Response().generate(apouche::HTTPVersionNotSupported, nullptr));

     if (!request->getHeaders()->containsHeader("host"))
         throw zia::http_exception("Unknown host",
                                       zia::httpEngine::Response().generate(apouche::BadRequest, nullptr));
     return true;
 })