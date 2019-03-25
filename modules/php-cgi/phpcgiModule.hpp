#pragma once

#include <functional>
#include <Event/Event.hpp>
#include <HttpConf/IHttpConf.hpp>
#include <HttpRequest/IHttpRequest.hh>
#include <HttpResponse/IHttpResponse.hh>
#include <Module/AModule.hpp>

namespace apouche
{
    class phpcgiModule : public AModule
    {
        using FunctionCGI = std::function<void(IHttpRequest *, IHttpResponse *, IHttpConf *)>;
        using EventCGI = Event<void(IHttpRequest *, IHttpResponse *, IHttpConf *)>;

    public:
        phpcgiModule(apouche::Logger *log = new apouche::Logger()) : AModule("PHP-CGI",
                                                                             "PHP file handler using PHP-CGI",
                                                                             "1.0.0", log)
        {
            methodName_ =
                    {
                            {apouche::Method::Post,                   "POST"},
                            {apouche::Method::Get,                    "GET"},
                            {apouche::Method::Options,                "OPTIONS"},
                            {apouche::Method::Head,                   "HEAD"},
                            {apouche::Method::Put,                    "PUT"},
                            {apouche::Method::Delete,                 "DELETE"},
                            {apouche::Method::Trace,                  "TRACE"},
                            {apouche::Method::Connect,                "CONNECT"},
                            {apouche::Method::UndefinedRequestMethod, "UNDEFINED"}
                    };

            statusList_ = {
                    apouche::StatusCode::Continue,
                    apouche::StatusCode::SwitchingProtocols,
                    apouche::StatusCode::OK,
                    apouche::StatusCode::Created,
                    apouche::StatusCode::Accepted,
                    apouche::StatusCode::NonAuthoritativeInformation,
                    apouche::StatusCode::NoContent,
                    apouche::StatusCode::ResetContent,
                    apouche::StatusCode::PartialContent,
                    apouche::StatusCode::MultipleChoices,
                    apouche::StatusCode::MovedPermanently,
                    apouche::StatusCode::Found,
                    apouche::StatusCode::SeeOther,
                    apouche::StatusCode::NotModified,
                    apouche::StatusCode::UseProxy,
                    apouche::StatusCode::TemporaryRedirect,
                    apouche::StatusCode::BadRequest,
                    apouche::StatusCode::Unauthorized,
                    apouche::StatusCode::PaymentRequired,
                    apouche::StatusCode::Forbidden,
                    apouche::StatusCode::NotFound,
                    apouche::StatusCode::MethodNotAllowed,
                    apouche::StatusCode::NotAcceptable,
                    apouche::StatusCode::ProxyAuthenticationRequired,
                    apouche::StatusCode::RequestTimeOut,
                    apouche::StatusCode::Conflict,
                    apouche::StatusCode::Gone,
                    apouche::StatusCode::LengthRequired,
                    apouche::StatusCode::PreconditionFailed,
                    apouche::StatusCode::RequestEntityTooLarge,
                    apouche::StatusCode::RequestURITooLarge,
                    apouche::StatusCode::UnsupportedMediaType,
                    apouche::StatusCode::RequestedRangeNotSatisfiable,
                    apouche::StatusCode::ExpectationFailed,
                    apouche::StatusCode::InternalServerError,
                    apouche::StatusCode::NotImplemented,
                    apouche::StatusCode::BadGateway,
                    apouche::StatusCode::ServiceUnavailable,
                    apouche::StatusCode::GatewayTimeOut,
                    apouche::StatusCode::HTTPVersionNotSupported
            };
        }

        void registerEvents(EventHandler *);
        void cgiRequest(IHttpRequest *, IHttpResponse *, IHttpConf *);
        void setCGIheader(const std::string &, IHttpResponse *);
        std::string getCGIbody(const std::string &);
        std::string getConf(const std::string &, const std::string &, IHttpConf *);

    private:
        std::map<apouche::Method, const std::string> methodName_;
        std::array<apouche::StatusCode, 41>          statusList_;
    };
}
