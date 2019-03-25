#include <iostream>
#include "exception/custom_exception.hh"

namespace zia
{
    custom_exception::custom_exception(std::string const &what) : what_{what} {}

    const char *custom_exception::what() const throw() {
        return what_.c_str();
    }
}