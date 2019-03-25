#include <fstream>
#include "exception/io_exception.hh"
#include "io/ioWriter.hh"

namespace zia
{
    namespace io
    {
        ioWriter::ioWriter(std::string const &fileName, std::mutex &m) : file_{fileName}, m_{&m} {}

        void ioWriter::writeInFile(std::string const &str) const
        {
            std::lock_guard<std::mutex>(*m_);
            std::ofstream oss{file_, std::ios::app};
            if (!oss)
            {
                throw zia::io_exception{"Cannot open file named : " + file_};
            }
            oss << str;
        }
    }
}