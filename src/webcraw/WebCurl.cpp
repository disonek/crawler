#include "WebCurl.hpp"

#include <curl/curl.h>

#include <stdexcept>
#include <string>
#include <cpr/cpr.h>


std::string WebCurl::getPage(const std::string& url)
{
    cpr::Response r = cpr::Get(cpr::Url{url}, 
                    cpr::VerifySsl(false),
                    cpr::Parameters{{"anon", "true"}, {"key", "value"}});
                      
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    return r.text;
}
