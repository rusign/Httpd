#pragma once

#include <mutex>
#include <string>

namespace zia
{
    namespace io
    {
        class ioReader
        {
        public:
            ioReader(std::string const &, std::mutex &);
            std::string readFromFile() const;
        private:
            std::string const file_;
            std::mutex        *m_;
        };
    }
}