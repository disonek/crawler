#include "WebCurl.hpp"

#include "Timer.hpp"
#include "Trace.hpp"

cpr::Response WebCurl::getPage(const std::string& url)
{
    InstrumentationTimer timer(__func__);
    cpr::Response r = cpr::Get(cpr::Url{url}, cpr::VerifySsl(false));
    r.status_code;
    r.header["content-type"];
    r.text;

    return r;
}
