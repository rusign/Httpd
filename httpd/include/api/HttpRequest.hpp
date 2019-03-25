#pragma once

#include <HttpBody/HttpBody.hh>
#include <HttpHeader/HttpHeader.hh>
#include <HttpRequest/IHttpRequest.hh>

namespace zia
{
    using UriParameters = std::map<std::string, std::string>;

    class HttpRequest : public apouche::IHttpRequest
    {
    public:
        HttpRequest(const std::string &);
        HttpRequest(const zia::HttpRequest &request) : HttpRequest(&request) { }
        HttpRequest(const apouche::IHttpRequest *);
        ~HttpRequest() { delete header_; delete body_; }
        void operator=(const HttpRequest &) = delete;

        const std::string serialize() const;

        apouche::Method getMethod() const { return method_; }
        void setMethod(apouche::Method method) { method_ = method; }

        const std::string &getURI() const { return uri_; }
        const std::string &getURL() const { return url_; }
        void setURI(const std::string &);

        const UriParameters &getUriParameters() const { return params_; }
        void setUriParameters(const UriParameters &uriParameters) {}
        bool containsUriParameters(const std::string &key) const { return params_.find(key) != params_.end(); }

        const std::string &getUriParameter(const std::string &) const;
		void setUriParameter(const std::string &key, const std::string &value) { params_[key] = value; }

        const std::string getRequestLine() const;
        const std::string getRequestLine();


        apouche::IHttpHeader *getHeaders() { return header_; }
        const apouche::IHttpHeader *getHeaders() const { return header_; }
        void setHeaders(apouche::IHttpHeader *header) {}

        apouche::IHttpBody *getBody() { return body_; }
        const apouche::IHttpBody *getBody() const { return body_; }
        void setBody(apouche::IHttpBody *body) {}

        const std::string &getVersion() const { return version_; }
        void setVersion(const std::string &version) {}

    private:
        apouche::Method method_;
        std::string     uri_;
        std::string     url_;
        UriParameters   params_;

        std::string         version_;
        apouche::HttpHeader *header_;
        apouche::HttpBody   *body_;
    };
}