#include "api/HttpResponse.hpp"
#include "custom_exception.hh"

namespace zia
{
    class http_exception : public custom_exception
    {
    public:
        http_exception(const std::string &what, const zia::HttpResponse &response) :
                custom_exception{what},
                response_(new HttpResponse(&response)) {}
        ~http_exception() { delete response_; }
        zia::HttpResponse *getResponse() const { return response_; };

    private:
        zia::HttpResponse *response_;
    };
}