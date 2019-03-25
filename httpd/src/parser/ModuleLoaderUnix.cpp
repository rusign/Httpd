#include <dlfcn.h>
#include <Module/AModule.hpp>
#include "exception/conf_exception.hh"
#include "utils/String.hh"
#include "parser/YamlParser.hpp"

const std::string zia::YamlParser::loaderLastError(const std::string &) const
{
    return std::string("Failed to charge module from ") + dlerror();
}

void zia::YamlParser::loadModules(const std::map<std::string, std::string> &conf) TRACE_EXCEPTION
({
     zia::ConfErrorList errorList;
     std::string        fnc_name   = "instantiate";
     std::string        fncWL_name = "instantiateWithLogger";
     auto               modules    = listModules(conf);

     for (auto path : modules)
     {
         auto hndl = dlopen(path.c_str(), RTLD_LAZY);
         if (hndl == nullptr)
         {
             errorList.push_back(loaderLastError(path));
             continue;
         }

         auto fncWL = reinterpret_cast<apouche::AModule *(*)(apouche::Logger *)>(dlsym(hndl, fncWL_name.c_str()));
         if (fncWL == nullptr)
         {
             auto fnc = reinterpret_cast<apouche::AModule *(*)()>(dlsym(hndl, fnc_name.c_str()));
             if (fnc == nullptr)
             {
                 errorList.push_back(loaderLastError(path));
                 continue;
             }

             auto module = fnc();
             module->registerEvents(&_httpEvents);
//             delete module;
             continue;
         }

         auto module = fncWL(_logger);
         module->registerEvents(&_httpEvents);
//         delete module;
     }
     if (!errorList.empty())
         throw zia::conf_exception(errorList);
 })