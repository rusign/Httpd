//
// Created by nicolas rusig on 01/02/2017.
//

#pragma once

#include "HttpConf/IHttpConf.hpp"

namespace zia
{
    class HttpConf : public apouche::IHttpConf
    {

    public:
        ~HttpConf() {};

        const std::map<std::string, std::string> &getAllConf() const;
        void setAllConf(const std::map<std::string, std::string> &);

        const std::string &getConf(const std::string &key) const;
        void setConf(const std::string &, const std::string &);

        void deleteConf(const std::string &);
        const bool containsConf(const std::string &) const;

        void update() {}
        void load() {}

    private:
        std::map<std::string, std::string> _conf;
    };
}