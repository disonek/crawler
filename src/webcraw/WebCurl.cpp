#include "WebCurl.hpp"

#include <cpr/cpr.h>
#include <ctype.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <string.h>

#include <stdexcept>
#include <string>


std::string WebCurl::getPage(const std::string& url)
{
    cpr::Response r = cpr::Get(cpr::Url{url}, cpr::VerifySsl(false));
    r.status_code;
    r.header["content-type"];
    r.text;

    return r.text;
}
