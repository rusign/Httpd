#include "exception/http_exception.hh"
#include "httpEngine/Response.hh"
#include "httpEngine/Engine.hh"
#include "utils/String.hh"

const zia::HttpResponse zia::httpEngine::Engine::Get(const zia::HttpRequest &request,
                                                     const std::string &host,
                                                     const std::string &directory) const TRACE_EXCEPTION
({
     auto fvPath     = "sites:" + host + ":folderview";
     auto directory_ = std::string(directory);

     if (zia::utils::String().endWith(directory_, "/"))
     directory_ = directory.substr(0, directory_.size() - 1);
     return readFromFile(request, directory_, conf_->containsConf(fvPath) && conf_->getConf(fvPath) == "true");
 })