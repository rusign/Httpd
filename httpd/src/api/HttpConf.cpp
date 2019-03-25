#include "api/HttpConf.h"

const std::map<std::string, std::string> &zia::HttpConf::getAllConf() const
{
    return _conf;
}

void zia::HttpConf::setAllConf(const std::map<std::string, std::string> &conf)
{
    _conf = conf;
}

const std::string &zia::HttpConf::getConf(const std::string &key) const
{
    return _conf.at(key);
}

void zia::HttpConf::setConf(const std::string &key, const std::string &value)
{
    _conf[key] = value;
}

void zia::HttpConf::deleteConf(const std::string &value)
{
    std::map<std::string, std::string>::iterator it;

    if ((it = _conf.find(value)) != _conf.end())
        _conf.erase(it);
}

const bool zia::HttpConf::containsConf(const std::string &value) const
{
    return (_conf.find(value) != _conf.end());
}