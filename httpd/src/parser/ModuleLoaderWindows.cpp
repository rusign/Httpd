#include <locale>
#include <Module/AModule.hpp>
#include "exception/conf_exception.hh"
#include "parser/YamlParser.hpp"

#define ERROR_SIZE    ((DWORD)512)

const std::string zia::YamlParser::loaderLastError(const std::string &path) const
{
    CHAR errorMessage[ERROR_SIZE];

    auto errorId = GetLastError();
    if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorId, 0, errorMessage, ERROR_SIZE, nullptr))
        return "Failed to charge module from " + path + " (No message found)";
    return "Failed to charge module from " + path + ": " + errorMessage;
}

void zia::YamlParser::loadModules(const std::map<std::string, std::string> &conf)
{
    zia::ConfErrorList errorList;
    std::string        fnc_name   = "instantiate";
    std::string        fncWL_name = "instantiateWithLogger";
    auto               modules    = listModules(conf);

    for (auto path : modules)
    {
        auto hndl = LoadLibrary(path.c_str());
        if (hndl == nullptr)
        {
            errorList.push_back(loaderLastError(path));
            continue;
        }

        auto fncWL = reinterpret_cast<apouche::AModule *(*)(apouche::Logger *)>(GetProcAddress(hndl, fncWL_name.c_str()));
        if (fncWL == nullptr)
        {
            auto fnc = reinterpret_cast<apouche::AModule *(*)()>(GetProcAddress(hndl, fnc_name.c_str()));
            if (fnc == nullptr)
            {
                errorList.push_back(loaderLastError(path));
                continue;
            }

            auto module = fnc();
            module->registerEvents(&_httpEvents);
            delete module;
        }

        auto module = fncWL(_logger);
        module->registerEvents(&_httpEvents);
        delete module;
    }
    if (!errorList.empty())
        throw zia::conf_exception(errorList);
}
