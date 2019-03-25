#include <string>

namespace zia
{
    namespace utils
    {
        class String
        {
        public:
            bool startWith(const std::string &, const std::string &);
            bool endWith(const std::string &, const std::string &);
        };
    }
}