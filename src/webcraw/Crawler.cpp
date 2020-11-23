#include "Crawler.hpp"

#include <libxml/HTMLparser.h>
#include <libxml/uri.h>
#include <libxml/xpath.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <future>
#include <iostream>

#include "Timer.hpp"
#include "Webcurl.hpp"

namespace webcrawler {

Crawler::Crawler(uint8_t numThreads)
    : numThreads{numThreads}
{
}

std::set<std::string> Crawler::getLinksFromUrl(const std::string url)
{
    InstrumentationTimer timer(__func__);
    auto pageContent = WebCurl::getPage(url);
    if(200 != pageContent.status_code)
    {
        return {};
    }

    return extractLinks(pageContent.text, url);
}

std::set<std::string> Crawler::extractLinks(std::string response, std::string url)
{
    // std::cout << "this_thread is " << std::hash<std::thread::id>{}(std::this_thread::get_id()) << "\n";
    InstrumentationTimer timer(__func__);
    std::set<std::string> foundLinks;
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

void Crawler::crawl(std::set<std::string> initialRequests)
{
    std::copy(initialRequests.begin(), initialRequests.end(), std::back_inserter(requestsToDo));

    while(!requestsToDo.empty())
    {
        std::vector<std::future<std::set<std::string>>> futures;
        for(int i = 0; i < numThreads; i++)
        {
            if(!requestsToDo.empty())
            {
                futures.push_back(std::async(&Crawler::getLinksFromUrl, this, requestsToDo.front()));
                requestsDone.insert(requestsToDo.front());
                requestsToDo.pop_front();
            }
        }

        for(auto& future : futures)
        {
            std::set<std::string> r1 = future.get();
        }

        // for(auto& future : futures)
        // {
        //     std::set<std::string> diff;
        //     std::set<std::string> r1 = future.get();
        //     std::set_difference(
        //         r1.begin(), r1.end(), requestsDone.begin(), requestsDone.end(), std::back_inserter(requestsToDo));
        // }

        spdlog::info("requestsToDo size= {}", requestsToDo.size());
        spdlog::info("requestsDone size= {}", requestsDone.size());
    }

    // for(auto request : requestsToDo)
    //     spdlog::info("requestsToDo {}", request.c_str());
    // for(auto request : requestsDone)
    //     spdlog::info("requestsDone {}", request.c_str());
}

} // namespace webcrawler
