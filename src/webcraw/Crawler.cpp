#include "Crawler.hpp"

#include <libxml/HTMLparser.h>
#include <libxml/uri.h>
#include <libxml/xpath.h>
#include <spdlog/spdlog.h>

#include <future>

#include "Webcurl.hpp"

namespace webcrawler {

void Crawler::start(const std::string& startURL)
{
    spdlog::info("Crawling {}", startURL);
    auto pageContent = WebCurl::getPage(startURL);
    if(200 != pageContent.status_code)
    {
        return;
    }

    std::unordered_set<std::string> links = extractLinks(pageContent.text, startURL);
    std::lock_guard<std::mutex> lock(mutex);
    for(auto link : links)
    {
        spdlog::error("link {}", link);
        if(link != startURL)
            requests.push(std::move(link));
    }
}

std::unordered_set<std::string> Crawler::extractLinks(std::string response, std::string url)
{
    std::unordered_set<std::string> foundLinks;
    int opts = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET;

    htmlDocPtr doc = htmlReadMemory(response.c_str(), response.size(), url.c_str(), NULL, opts);
    if(!doc)
        return foundLinks;

    xmlChar* xpath = (xmlChar*)"//a/@href";
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath, context);
    xmlXPathFreeContext(context);

    xmlNodeSetPtr nodeset = result->nodesetval;
    if(xmlXPathNodeSetIsEmpty(nodeset))
    {
        xmlXPathFreeObject(result);
        return foundLinks;
    }

    for(auto i = 0; i < nodeset->nodeNr; i++)
    {
        const xmlNode* node = nodeset->nodeTab[i]->xmlChildrenNode;
        xmlChar* href = xmlNodeListGetString(doc, node, 1);
        href = xmlBuildURI(href, (xmlChar*)url.c_str());

        char* foundLink = (char*)href;
        if(foundLink != nullptr)
            foundLinks.emplace(std::string{foundLink});
        xmlFree(foundLink);
    }
    xmlXPathFreeObject(result);
    xmlFreeDoc(doc);
    return foundLinks;
}

void Crawler::crawl()
{
    std::lock_guard<std::mutex> lock(mutex);
    while(!requests.empty())
    {
        spdlog::info("Crawling {}", requests.front().c_str());
        auto fut = std::async(WebCurl::getPage, requests.front());
        cpr::Response response = fut.get();

        if(200 != response.status_code)
        {
            return;
        }

        std::unordered_set<std::string> links = extractLinks(response.text, requests.front());

        for(auto link : links)
        {
            spdlog::error("link {}", link);
        }

        requests.pop();
    }
}
} // namespace webcrawler
