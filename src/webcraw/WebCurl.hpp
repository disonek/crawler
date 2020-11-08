#pragma once

#include <cpr/cpr.h>

#include <string>

class WebCurl
{
public:
    static cpr::Response getPage(const std::string& url);
};
