#include "io/ioReader.hh"
#include "exception/io_exception.hh"
#include <fstream>
#include <iostream>

namespace zia
{
    namespace io
    {
        ioReader::ioReader(std::string const &file, std::mutex &m) : file_{file}, m_{&m} {}

        std::string ioReader::readFromFile() const TRACE_EXCEPTION
        ({
            std::lock_guard<std::mutex> guard(*m_);
            std::ifstream               ifs{file_};

            if (!ifs)
                throw zia::io_exception{"Error, cannot open file named " + file_};
            try
            {
                return std::string((std::istreambuf_iterator<char>(ifs)),
                                   std::istreambuf_iterator<char>());
            }
            catch (std::exception &e) { throw zia::io_exception{"Failed to read file " + file_ + ":  " + e.what()}; }
        })
    }
}