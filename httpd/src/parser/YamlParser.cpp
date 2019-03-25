#include <cstring>
#include "utils/String.hh"
#include "exception/conf_exception.hh"
#include "parser/YamlParser.hpp"

zia::YamlParser &zia::YamlParser::getInstance()
{
    static YamlParser instance;
    return instance;
}
bool zia::YamlParser::restorePrevious()
{
    if (_lastConf.empty())
        return false;
    _httpConf.setAllConf(_lastConf);
    return true;
}

void zia::YamlParser::scanScalar(YAML::Node nodes, std::string path, std::map<std::string, std::string> &conf)
{
    for (auto idx = 0; idx < nodes.size(); ++idx)
    {
        auto path_ = path + (path.empty() ? "" : ":") + std::to_string(idx);

        if (nodes[idx].IsScalar())
        {
            std::stringstream ss;
            ss << nodes[idx];
            conf[path_] = ss.str();
        }
        else if (nodes[idx].IsMap())
            scan(nodes[idx], path_, conf);
        else
            scanScalar(nodes[idx], path_, conf);
    }
}

void zia::YamlParser::scan(YAML::Node nodes, std::string path, std::map<std::string, std::string> &conf)
{
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        std::stringstream ss;
        ss << it->first;
        auto path_ = path + (path.empty() ? "" : ":") + ss.str();
        ss.str("");
        if (it->second.IsScalar())
        {
            ss << it->second;
            conf[path_] = ss.str();
        }
        else if (it->second.IsMap())
            scan(it->second, path_, conf);
        else
            scanScalar(it->second, path_, conf);
    }
}

bool zia::YamlParser::loadFile(const std::string &file)
{
    auto conf = std::map<std::string, std::string>();

	struct stat result;
	if (stat(file.c_str(), &result) == 0)
		_previousTime = result.st_mtime;
	
	try
    {
        YAML::Node yaml = YAML::LoadFile(file);

        scan(yaml, "", conf);
        validateConf(conf);
        _httpEvents.clear();
        if (!_httpConf.getAllConf().empty())
            loadModules(conf);
    }
    catch (zia::conf_exception &e)
    {
        for (auto error : e.errors())
            _logger->error(std::string("[Configuration]\t") + error);
        return false;
    }
    catch (zia::custom_exception &e)
    {
        _logger->exception("Configuration", e);
        return false;
    }
    catch (std::exception &e)
    {
        if (errno)
            _logger->error(std::string("[Configuration]\t") + e.what() + " (" + strerror(errno) + ")");
        else
            _logger->error(std::string("[Configuration]\t") + e.what());
        return false;
    }

    _httpConf.setAllConf(conf);
    return true;
}

void zia::YamlParser::validateConf(const std::map<std::string, std::string> &conf) const
{
    zia::utils::String          stringtool;
    zia::ConfErrorList          errorList;
    std::map<std::string, bool> sites;
    std::string                 site_pattern = "sites:";

    if (conf.find("server:log:file") == conf.end())
        errorList.push_back("'logfile' path not set (server:log:file)");

    for (auto line : conf)
        if (stringtool.startWith(line.first, site_pattern))
        {
            auto   site = line.first.substr(site_pattern.size());
            size_t pos;
            if ((pos = site.find(':')) != std::string::npos)
                site = site.substr(0, pos);

            if (sites.find(site) == sites.end())
                sites[site] = false;
            if (stringtool.endWith(line.first, "directory"))
                sites[site] = true;
        }

    for (auto site : sites)
        if (!site.second)
            errorList.push_back(site.first + ": 'directory' path not set (sites:" + site.first + ":directory)");

    TRACE_EXCEPTION
    (
            if (!errorList.empty())
                throw zia::conf_exception(errorList);
    )
}

std::list<std::string> zia::YamlParser::listModules(const std::map<std::string, std::string> &conf) const
{
	zia::utils::String     stringtool;
	std::string            network_pattern = "server:modules:network:path";
	std::string            treatment_pattern = "server:modules:treatment:";
	std::list<std::string> modules;

	for (auto module : conf)
		if (stringtool.startWith(module.first, network_pattern) ||
			stringtool.startWith(module.first, treatment_pattern))
			modules.push_front(module.second);
	return modules;
}