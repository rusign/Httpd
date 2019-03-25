#pragma once

#include <HttpBody/HttpBody.hh>
#include <HttpHeader/HttpHeader.hh>
#include <HttpResponse/IHttpResponse.hh>
#include "utils/Converter.hh"

namespace zia
{
    class HttpResponse : public apouche::IHttpResponse
    {
    public:
        HttpResponse(const std::string &);
        HttpResponse(const zia::HttpResponse &response) : HttpResponse(&response) { }
        HttpResponse(const apouche::IHttpResponse *);
        ~HttpResponse() { delete header_; delete body_; }
        void operator=(const HttpResponse &) = delete;

        const std::string serialize() const;

        apouche::StatusCode getStatus() const { return status_; }
        void setStatus(apouche::StatusCode status) { status_ = status; }
        const std::string &getStatusDescription() const { return zia::utils::Converter().toString(getStatus()); }
        const std::string getResponseLine() const;

        apouche::IHttpHeader *getHeaders() { return header_; }
        const apouche::IHttpHeader *getHeaders() const { return header_; }
        void setHeaders(apouche::IHttpHeader *header) {}

        apouche::IHttpBody *getBody() { return body_; }
        const apouche::IHttpBody *getBody() const { return body_; }
        void setBody(apouche::IHttpBody *body) {}

        const std::string &getVersion() const { return version_; }
        void setVersion(const std::string &version) {}

    private:
        apouche::StatusCode status_;

        std::string         version_;
        apouche::HttpHeader *header_;
        apouche::HttpBody   *body_;
    };
}