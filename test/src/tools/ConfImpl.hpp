#include <HttpConf/IHttpConf.hpp>

namespace test
{
    class ConfImpl : public apouche::IHttpConf
    {
    public:
        const std::map<std::string, std::string> &getAllConf() const { return conf_; }
        void setAllConf(const std::map<std::string, std::string> &conf) { conf_ = conf; }
        const std::string &getConf(const std::string &key) const { return conf_.at(key); }
        void setConf(const std::string &k, const std::string &v) { conf_[k] = v; }
        void deleteConf(const std::string &) {}
        const bool containsConf(const std::string &k) const { return conf_.find(k) != conf_.end(); }
        void update() {}
        void load() {}

    private:
        std::map<std::string, std::string> conf_;

    };
}