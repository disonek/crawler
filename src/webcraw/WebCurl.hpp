#pragma once

#include <string>

class WebCurl {
public:
    static std::string getPage(const std::string &url);
};

