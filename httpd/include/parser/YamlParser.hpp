#pragma once

#ifndef _WIN32
	#include <unistd.h>
#endif


#include <stdio.h>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <yaml-cpp/yaml.h>
#include <EventHandler/EventHandler.hpp>
#include "api/Logger.hh"
#include "api/HttpConf.h"

namespace zia
{
    class YamlParser
    {
    public:
        static YamlParser &getInstance();

        bool loadFile(const std::string &);

        void setLogger(zia::Logger &logger) { _logger = &logger; }
        const zia::HttpConf &getHttpConf() const { return _httpConf; }
        bool restorePrevious();
        void saveConf() { _lastConf = _httpConf.getAllConf(); }
        const apouche::EventHandler &getHttpEvents() const { return _httpEvents; }
        time_t getpreviousTime() const { return _previousTime; }

    private:
        YamlParser(const YamlParser &) {}
        YamlParser() {}
        ~YamlParser() {}

        void scan(YAML::Node, std::string, std::map<std::string, std::string> &);
        void scanScalar(YAML::Node, std::string, std::map<std::string, std::string> &);

        void validateConf(const std::map<std::string, std::string> &) const;
        std::list<std::string> listModules(const std::map<std::string, std::string> &) const;

    	void loadModules(const std::map<std::string, std::string> &);
		const std::string loaderLastError(const std::string &) const;

        zia::HttpConf                      _httpConf;
        std::map<std::string, std::string> _lastConf;
        apouche::EventHandler              _httpEvents;
        zia::Logger                        *_logger;
        time_t                             _previousTime;
    };
}
