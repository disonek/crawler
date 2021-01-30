#pragma once

#include <set>
#include <string>

namespace mess {

struct CrawlRequest
{
    std::string url;
};

struct Continue
{
};

struct CrawlResponse
{
    std::set<std::string> response;
};

} // namespace mess