#include <algorithm>
#include <sstream>
#include "api/HttpRequest.hpp"
#include "exception/http_exception.hh"
#include "httpEngine/Grammar.hh"
#include "httpEngine/Request.hh"
#include "httpEngine/Response.hh"
#include "utils/Splitter.hh"
#include "utils/Trimmer.hh"
#include "utils/Uri.hh"

#include <iostream>

zia::HttpRequest::HttpRequest(const std::string &request) :
        header_(new apouche::HttpHeader()),
        body_(new apouche::HttpBody) TRACE_EXCEPTION
({
     zia::utils::Converter converter;
     zia::utils::Splitter  splitter;
     zia::utils::Trimmer   trimmer;
     std::string           request_(request);

     auto request_line = splitter.split(request_, httpEngine::Grammar::CRLF);
     method_ = converter.toMethod(splitter.split(request_line, zia::httpEngine::Grammar::SP));

     try { setURI(splitter.split(request_line, zia::httpEngine::Grammar::SP)); }
     catch (std::exception &e) { delete header_; delete body_; throw; }
     catch (std::exception *e) { delete header_; delete body_; throw; }


     version_ = request_line;

     std::string headerLine;
     while (headerLine != zia::httpEngine::Grammar::CRLF)
     {
         headerLine       = splitter.split(request_, zia::httpEngine::Grammar::CRLF);
         if (headerLine.empty())
             break;
         auto headerToken = splitter.split(headerLine, ":");
         std::transform(headerToken.begin(), headerToken.end(), headerToken.begin(), ::tolower);
         std::transform(headerLine.begin(), headerLine.end(), headerLine.begin(), ::tolower);
         header_->setHeader(trimmer.trim(headerToken), trimmer.trim(headerLine));
     }
     body_->setBody(request_);

     try { zia::httpEngine::Request().validate(this); }
     catch (std::exception &e) { delete header_; delete body_; throw; }
     catch (std::exception *e) { delete header_; delete body_; throw; }

 })

zia::HttpRequest::HttpRequest(const apouche::IHttpRequest *request) :
        header_(new apouche::HttpHeader()),
        body_(new apouche::HttpBody) TRACE_EXCEPTION
({
     try { zia::httpEngine::Request().validate(request); }
     catch (std::exception &e) { delete header_; delete body_; throw; }
     catch (std::exception *e) { delete header_; delete body_; throw; }

     if (request)
     {
         method_ = request->getMethod();
         try { setURI(request->getURI()); }
         catch (std::exception &e) { delete header_; delete body_; throw; }
         catch (std::exception *e) { delete header_; delete body_; throw; }

         params_  = request->getUriParameters();
         version_ = request->getVersion();
         if (request->getHeaders())
             header_->setAllHeaders(request->getHeaders()->getAllHeader());
         if (request->getBody())
             body_->setBody(request->getBody()->getBody());
     }
 })

const std::string zia::HttpRequest::serialize() const
{
    std::stringstream ss;

    ss << getRequestLine() << zia::httpEngine::Grammar::CRLF;
    for (auto it : header_->getAllHeader())
        ss << it.first << ":" << it.second << zia::httpEngine::Grammar::CRLF;
    ss << zia::httpEngine::Grammar::CRLF << body_->getBody();
    return ss.str();
}

void zia::HttpRequest::setURI(const std::string &uri)
{
    zia::utils::Splitter splitter;
    zia::utils::Trimmer  trimmer;
    zia::utils::Uri      uritool;
    std::string          uri_cpy(uri);

#ifndef _WIN32
    if (!uritool.validate(uri_cpy))
        throw zia::http_exception("Invalid URL",
                                  zia::httpEngine::Response().generate(apouche::BadRequest, nullptr));
#endif

    uri_ = uri;

    auto url  = splitter.split(uri_cpy, "?");
    auto host = splitter.split(url, "/");
    url_ = uritool.extractUrl(uri);

    while (!uri_cpy.empty())
    {
        std::string param = splitter.split(uri_cpy, "&");
        if (param.empty() && uri.find('&') == std::string::npos)
            break;
        else if (param.empty())
            continue;
        auto paramToken = splitter.split(param, "=");
        if (paramToken.empty())
            continue;
        paramToken = uritool.decode(paramToken);
        paramToken = trimmer.trim(param);
        param      = uritool.decode(param);
        param      = trimmer.trim(param);
        params_[paramToken] = param;
    }
    if (!host.empty() && !header_->containsHeader("host"))
        header_->setHeader("host", host);
}

const std::string &zia::HttpRequest::getUriParameter(const std::string &key) const
{
    if (params_.find(key) == params_.end())
        throw std::invalid_argument("parameter " + key + " not found");
    return params_.find(key)->second;
}

const std::string zia::HttpRequest::getRequestLine() const
{
    zia::utils::Converter converter;
    std::stringstream     ss;

    ss << converter.toString(getMethod())
       << zia::httpEngine::Grammar::SP << getURI()
       << zia::httpEngine::Grammar::SP << getVersion();
    return ss.str();
}
const std::string zia::HttpRequest::getRequestLine()
{
    return static_cast<const HttpRequest *>(this)->getRequestLine();
}
