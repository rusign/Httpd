#include <Enum.hh>
#include "httpEngine/Grammar.hh"

const std::string    zia::httpEngine::Grammar::CR(1, 13);
const std::string    zia::httpEngine::Grammar::LF(1, 10);
const std::string    zia::httpEngine::Grammar::SP(1, 32);
const std::string    zia::httpEngine::Grammar::HT(1, 9);
const std::string    zia::httpEngine::Grammar::CRLF(CR + LF);

const zia::httpEngine::StatusCodeName zia::httpEngine::Grammar::StatusCode =
                                              {
                                                      // Undefined		:: 0XX
                                                      {apouche::StatusCode::UndefinedStatusCode,          "Undefined Status Code"},

                                                      // Informational	:: 1XX
                                                      {apouche::StatusCode::Continue,                     "Continue"},
                                                      {apouche::StatusCode::SwitchingProtocols,           "Switching Protocols"},

                                                      // Success			:: 2XX
                                                      {apouche::StatusCode::OK,                           "OK"},
                                                      {apouche::StatusCode::Created,                      "Created"},
                                                      {apouche::StatusCode::Accepted,                     "Accepted"},
                                                      {apouche::StatusCode::NonAuthoritativeInformation,  "Non Authoritative Information"},
                                                      {apouche::StatusCode::NoContent,                    "No Content"},
                                                      {apouche::StatusCode::ResetContent,                 "Reset Content"},
                                                      {apouche::StatusCode::PartialContent,               "Partial Content"},

                                                      // Redirection		:: 3XX
                                                      {apouche::StatusCode::MultipleChoices,              "Multiple Choices"},
                                                      {apouche::StatusCode::MovedPermanently,             "Moved Permanently"},
                                                      {apouche::StatusCode::Found,                        "Found"},
                                                      {apouche::StatusCode::SeeOther,                     "See Other"},
                                                      {apouche::StatusCode::NotModified,                  "Not Modified"},
                                                      {apouche::StatusCode::UseProxy,                     "Use Proxy"},
                                                      {apouche::StatusCode::TemporaryRedirect,            "Temporary Redirect"},

                                                      // ClientError		:: 4XX
                                                      {apouche::StatusCode::BadRequest,                   "Bad Request"},
                                                      {apouche::StatusCode::Unauthorized,                 "Unauthorized"},
                                                      {apouche::StatusCode::PaymentRequired,              "Payment Required"},
                                                      {apouche::StatusCode::Forbidden,                    "Forbidden"},
                                                      {apouche::StatusCode::NotFound,                     "Not Found"},
                                                      {apouche::StatusCode::MethodNotAllowed,             "Method Not Allowed"},
                                                      {apouche::StatusCode::NotAcceptable,                "Not Acceptable"},
                                                      {apouche::StatusCode::ProxyAuthenticationRequired,  "Proxy Authentication Required"},
                                                      {apouche::StatusCode::RequestTimeOut,               "Request Time Out"},
                                                      {apouche::StatusCode::Conflict,                     "Conflict"},
                                                      {apouche::StatusCode::Gone,                         "Gone"},
                                                      {apouche::StatusCode::LengthRequired,               "Length Required"},
                                                      {apouche::StatusCode::PreconditionFailed,           "Precondition Failed"},
                                                      {apouche::StatusCode::RequestEntityTooLarge,        "Request Entity Too Large"},
                                                      {apouche::StatusCode::RequestURITooLarge,           "Request URI Too Large"},
                                                      {apouche::StatusCode::UnsupportedMediaType,         "Unsupported Media Type"},
                                                      {apouche::StatusCode::RequestedRangeNotSatisfiable, "Requested Range Not Satisfiable"},
                                                      {apouche::StatusCode::ExpectationFailed,            "Expectation Failed"},

                                                      // ServerError		:: 5XX
                                                      {apouche::StatusCode::InternalServerError,          "Internal Server Error"},
                                                      {apouche::StatusCode::NotImplemented,               "Not Implemented"},
                                                      {apouche::StatusCode::BadGateway,                   "Bad Gateway"},
                                                      {apouche::StatusCode::ServiceUnavailable,           "Service Unavailable"},
                                                      {apouche::StatusCode::GatewayTimeOut,               "Gateway TimeOut"},
                                                      {apouche::StatusCode::HTTPVersionNotSupported,      "HTTP Version Not Supported"},
                                              };

const zia::httpEngine::MethodName zia::httpEngine::Grammar::Method =
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

const std::string    zia::httpEngine::Grammar::Date::weekday = "%a";
const std::string    zia::httpEngine::Grammar::Date::month   = "%b";
const std::string    zia::httpEngine::Grammar::Date::date3   = "" _(Date::month) " %d";
const std::string    zia::httpEngine::Grammar::Date::date2   = "%d-" _(Date::month) "-%Y";
const std::string    zia::httpEngine::Grammar::Date::date1   = "%d " _(Date::month) " %Y";
const std::string    zia::httpEngine::Grammar::Date::time    = "%H:%M:%S";

const std::string    zia::httpEngine::Grammar::Date::asciitime_date =
                             "" _(Date::weekday) " " _(Date::date3) " " _(Date::time) " %Y";
const std::string    zia::httpEngine::Grammar::Date::rfc850_date    =
                             "" _(Date::weekday) ", " _(Date::date2) " " _(Date::time) " GMT";
const std::string    zia::httpEngine::Grammar::Date::rfc1123_date   =
                             "" _(Date::weekday) ", " _(Date::date1) " " _(Date::time) " GMT";