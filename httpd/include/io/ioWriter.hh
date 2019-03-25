#pragma once

#include <mutex>
#include <string>

namespace zia
{
    namespace io
    {
        class ioWriter
        {
        public:
            ioWriter(std::string const &, std::mutex &);
            void writeInFile(std::string const &) const;
        private:
            std::string const file_;
            std::mutex        *m_;
        };
    }
}