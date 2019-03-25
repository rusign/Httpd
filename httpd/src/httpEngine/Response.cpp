#include <regex>
#include "exception/http_exception.hh"
#include "httpEngine/Engine.hh"
#include "httpEngine/Response.hh"
#include "utils/DateTime.hh"

#ifdef WIN32
	#define LENGTH_LIMIT ULLONG_MAX
#else
	#define LENGTH_LIMIT std::numeric_limits<uint64_t>::max()
#endif

bool zia::httpEngine::Response::validate(const apouche::IHttpResponse *response) TRACE_EXCEPTION
({

    if (response == nullptr || response->getHeaders() == nullptr || response->getBody() == nullptr)
        throw zia::http_exception("Invalid response instance",
                                  zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));

    if (response->getStatus() == apouche::UndefinedStatusCode)
        throw zia::http_exception("Undefined response status code",
                                  zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));

    std::regex version("HTTP\\/\\d+.\\d+");
    if (!std::regex_match(response->getVersion(), version))
        throw zia::http_exception("Invalid HTTP response version",
                                  zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));

    if (!response->getHeaders()->containsHeader("content-length") &&
        !response->getHeaders()->containsHeader("transfer-encoding"))
        throw zia::http_exception("Response content-length required",
                                  zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));
    else if (response->getHeaders()->containsHeader("content-length"))
    {
        uint64_t lenght = LENGTH_LIMIT;

        try
        {
            lenght = std::stoull(response->getHeaders()->getHeader("content-length"));
        }
        catch (...) {}

        if (lenght != response->getBody()->getBody().size())
            throw zia::http_exception("Invalid response content-length (Size not match)",
                                      zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));
    }
    else if (response->getHeaders()->containsHeader("transfer-encoding") &&
             response->getHeaders()->getHeader("transfer-encoding") == "chunked")
        throw zia::http_exception("Invalid response content-length ('chunked' not implemented)",
                                  zia::httpEngine::Response().generate(apouche::InternalServerError, nullptr));

    return true;
})

zia::HttpResponse zia::httpEngine::Response::generate(apouche::StatusCode status,
                                                      const std::function<void(
                                                              zia::HttpResponse &)> &f) const TRACE_EXCEPTION
({
     std::string       statusLine = std::to_string(status) +
                                    zia::httpEngine::Grammar::SP +
                                    zia::utils::Converter().toString(status);
     std::string       serverLine = std::string(SERVER_NAME) + "/" + SERVER_VERSION;
     std::string       content    = "<html><head><title>" +
                                    statusLine +
                                    "</title></head><body bgcolor=\"white\"><center><h1>" +
                                    statusLine +
                                    "</h1></center><hr><center>" +
                                    serverLine +
                                    "</center></body></html>";
     std::stringstream response_;

     response_ << "HTTP/1.1 " << statusLine << zia::httpEngine::Grammar::CRLF
               << "Server: " << serverLine << zia::httpEngine::Grammar::CRLF
               << "Date: " << zia::utils::DateTime().currentHttpTime() << zia::httpEngine::Grammar::CRLF
               << "Connection: keep-alive" << zia::httpEngine::Grammar::CRLF
               << "Content-Type: text/html" << zia::httpEngine::Grammar::CRLF
               << "Content-Length: " << content.size() << zia::httpEngine::Grammar::CRLF
               << zia::httpEngine::Grammar::CRLF
               << content;
     auto response = zia::HttpResponse(response_.str());

     if (f)
         f(response);
     return response;
 })
