#include "custom_exception.hh"

namespace zia
{
    using ConfErrorList = std::list<std::string>;

    class conf_exception : public custom_exception
    {
    public:
        conf_exception(const ConfErrorList &errors) : errors_(errors),
                                                                 custom_exception{"Undefined error, see errors list"} {}

        const ConfErrorList &errors() const { return errors_; }

    private:
        const ConfErrorList errors_;
    };
}