#include <algorithm>
#include <sstream>
#include <HttpBody/HttpBody.hh>
#include <HttpHeader/HttpHeader.hh>
#include "api/HttpResponse.hpp"
#include "exception/http_exception.hh"
#include "httpEngine/Grammar.hh"
#include "httpEngine/Response.hh"
#include "utils/Splitter.hh"
#include "utils/Trimmer.hh"

zia::HttpResponse::HttpResponse(const std::string &response) :
        header_(new apouche::HttpHeader()),
        body_(new apouche::HttpBody) TRACE_EXCEPTION
({
     zia::utils::Converter converter;
     zia::utils::Splitter  splitter;
     zia::utils::Trimmer   trimmer;
     std::string           response_(response);

     auto status_line = splitter.split(response_, zia::httpEngine::Grammar::CRLF);
     version_ = splitter.split(status_line, zia::httpEngine::Grammar::SP);
     status_  = converter.toStatusCode(splitter.split(status_line, zia::httpEngine::Grammar::SP));

     std::string headerLine;
     while (headerLine != zia::httpEngine::Grammar::CRLF)
     {
         headerLine       = splitter.split(response_, zia::httpEngine::Grammar::CRLF);
         if (headerLine.empty())
             break;
         auto headerToken = splitter.split(headerLine, ":");
         std::transform(headerToken.begin(), headerToken.end(), headerToken.begin(), ::tolower);
         header_->setHeader(trimmer.trim(headerToken), trimmer.trim(headerLine));
     }
     body_->setBody(response_);

     try { zia::httpEngine::Response().validate(this); }
     catch (std::exception &e) { delete header_; delete body_; throw; }
     catch (std::exception *e) { delete header_; delete body_; throw; }
 })

zia::HttpResponse::HttpResponse(const apouche::IHttpResponse *response) :
        header_(new apouche::HttpHeader()),
        body_(new apouche::HttpBody) TRACE_EXCEPTION
({
     try { zia::httpEngine::Response().validate(response); }
     catch (std::exception &e) { delete header_; delete body_; throw; }
     catch (std::exception *e) { delete header_; delete body_; throw; }

     if (response)
     {
         status_  = response->getStatus();
         version_ = response->getVersion();
         if (response->getHeaders())
             header_->setAllHeaders(response->getHeaders()->getAllHeader());
         if (response->getBody())
             body_->setBody(response->getBody()->getBody());
     }
 })

const std::string zia::HttpResponse::serialize() const
{
    std::stringstream ss;

    ss << getResponseLine() << zia::httpEngine::Grammar::CRLF;
    for (auto it : header_->getAllHeader())
        ss << it.first << ":" << it.second << zia::httpEngine::Grammar::CRLF;
    ss << zia::httpEngine::Grammar::CRLF << body_->getBody();

    return ss.str();
}

const std::string zia::HttpResponse::getResponseLine() const
{
    std::stringstream ss;

    ss << getVersion()
       << zia::httpEngine::Grammar::SP << getStatus()
       << zia::httpEngine::Grammar::SP << getStatusDescription();
    return ss.str();
}
