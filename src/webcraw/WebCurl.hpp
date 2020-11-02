#ifndef WCURL_H
#define WCURL_H

#include <string>

class WebCurl {
public:
    static std::string getPage(const std::string &url);
};

#endif // WCURL_H
