#include "WebCurl.hpp"

#include "ScopedTimer.hpp"
#include "TraceTimer.hpp"

cpr::Response WebCurl::getPage(const std::string& url)
{
    utils::TraceTimer timer(url.c_str());
    utils::ScopedTimer st(__func__, __func__);
    cpr::Response r = cpr::Get(cpr::Url{url}, cpr::VerifySsl(false));
    r.status_code;
    r.header["content-type"];
    r.text;

    return r;
}
