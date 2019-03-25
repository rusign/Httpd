#include "httpEngine/Grammar.hh"

using namespace zia;

namespace test
{
    class RequestGenerator
    {
    public:
        static const std::string simple_get()
        {
            std::stringstream ss;

            ss << "GET" << httpEngine::Grammar::SP << "/index.html" << httpEngine::Grammar::SP << "HTTP/1.1" << httpEngine::Grammar::CRLF;
            ss << "Host: localhost" << httpEngine::Grammar::CRLF;
            ss << "User-Agent: Google Unitary Test - PreProd zia"
               << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Encoding: gzip, deflate, br" << httpEngine::Grammar::CRLF;
            ss << "Connection: keep-alive" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }
        static const std::string simple_get_response()
        {
            return "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>Hello World</title></head><body></body></html>";
        }

        static const std::string simple_get_with_query()
        {
            std::stringstream ss;

            ss << "GET" << httpEngine::Grammar::SP << "zia.aiz/index.html?query=12&fun=0" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0"
               << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Encoding: gzip, deflate, br" << httpEngine::Grammar::CRLF;
            ss << "Connection: keep-alive" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }
        static const std::string simple_get_with_query_response()
        {
            return "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>Nothing</title></head><body></body></html>";
        }

        static const std::string only_get()
        {
            std::stringstream ss;

            ss << "GET" << httpEngine::Grammar::SP << "www.google.fr/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }

        static const std::string simple_post()
        {
            std::stringstream ss;

            ss << "POST" << httpEngine::Grammar::SP << "/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << "Host: www.google.fr" << httpEngine::Grammar::CRLF;
            ss << "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0"
               << httpEngine::Grammar::CRLF;
            ss << "Content-Length:5" << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Language: en-US,en;q=0.5" << httpEngine::Grammar::CRLF;
            ss << "Accept-Encoding: gzip, deflate, br" << httpEngine::Grammar::CRLF;
            ss << "Connection: keep-alive" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            ss << "azert";
            return ss.str();
        }

        static const std::string chunked_post()
        {
            std::stringstream ss;

            ss << "POST" << httpEngine::Grammar::SP << "/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << "Host: www.google.fr" << httpEngine::Grammar::CRLF;
            ss << "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0"
               << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Language: en-US,en;q=0.5" << httpEngine::Grammar::CRLF;
            ss << "Accept-Encoding: gzip, deflate, br" << httpEngine::Grammar::CRLF;
            ss << "Connection: keep-alive" << httpEngine::Grammar::CRLF;
            ss << "Transfert-Encoding: chunked" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            ss << "10" << httpEngine::Grammar::CRLF;
            ss << "azertyuiop" << httpEngine::Grammar::CRLF;
            ss << "2" << httpEngine::Grammar::CRLF;
            ss << "az" << httpEngine::Grammar::CRLF;
            ss << "0" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }

        static const std::string chunked_post_with_trailer()
        {
            std::stringstream ss;

            ss << "POST" << httpEngine::Grammar::SP << "/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Language: en-US,en;q=0.5" << httpEngine::Grammar::CRLF;
            ss << "Accept-Encoding: gzip, deflate, br" << httpEngine::Grammar::CRLF;
            ss << "Connection: keep-alive" << httpEngine::Grammar::CRLF;
            ss << "Transfert-Encoding: chunked" << httpEngine::Grammar::CRLF;
            ss << "Trailer: Host, User-Agent" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            ss << "10" << httpEngine::Grammar::CRLF;
            ss << "azertyuiop" << httpEngine::Grammar::CRLF;
            ss << "2" << httpEngine::Grammar::CRLF;
            ss << "az" << httpEngine::Grammar::CRLF;
            ss << "0" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            ss << "Host: www.google.fr" << httpEngine::Grammar::CRLF;
            ss << "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0"
               << httpEngine::Grammar::CRLF;
            return ss.str();
        }

        static const std::string post_error_411()
        {
            std::stringstream ss;

            ss << "POST" << httpEngine::Grammar::SP << "/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Language: en-US,en;q=0.5" << httpEngine::Grammar::CRLF;
            ss << "Accept-Encoding: gzip, deflate, br" << httpEngine::Grammar::CRLF;
            ss << "Connection: keep-alive" << httpEngine::Grammar::CRLF;
            ss << "Trailer: Host, User-Agent" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            ss << "10" << httpEngine::Grammar::CRLF;
            ss << "azertyuiop" << httpEngine::Grammar::CRLF;
            ss << "2" << httpEngine::Grammar::CRLF;
            ss << "az" << httpEngine::Grammar::CRLF;
            ss << "0" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }

        static const std::string get_error_400()
        {
            std::stringstream ss;

            ss << "POST" << httpEngine::Grammar::SP << "/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" << httpEngine::Grammar::CRLF;
            ss << "Accept-Language" << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }

        static const std::string get_error_400_2()
        {
            std::stringstream ss;

            ss << "POST" << httpEngine::Grammar::SP << "/" << httpEngine::Grammar::SP << "HTTP/1.1"
               << httpEngine::Grammar::CRLF;
            ss << httpEngine::Grammar::CRLF;
            return ss.str();
        }
    };
}