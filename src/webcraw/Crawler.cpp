#include "Crawler.hpp"

#include <libxml/HTMLparser.h>
#include <libxml/uri.h>
#include <libxml/xpath.h>
#include <spdlog/spdlog.h>

#include <future>

#include "ScopedTimer.hpp"
#include "Webcurl.hpp"

namespace webcrawler {
std::set<std::string> Crawler::CrawlerThread(TaskQueue& taskQueue, std::string url)
{
    utils::ScopedTimer timer(__func__, "main");
    utils::Trace::get().beginSession("Session Name");
    uint8_t numThreads = 15;
    auto crawler = webcrawler::Crawler{numThreads};
    std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);
    auto result = crawler.crawl(initialRequests);

    utils::Trace::get().endSession();
    std::lock_guard<std::mutex> lk(taskQueue.mutex);
    taskQueue.tasks.push(result);

    return result;
}

Crawler::Crawler(uint8_t numThreads)
    : numThreads{numThreads}
{
}

std::set<std::string> Crawler::getLinksFromUrl(const std::string url)
{
    auto pageContent = WebCurl::getPage(url);
    if(200 != pageContent.status_code)
    {
        return {};
    }

    return extractLinks(pageContent.text, url);
}

std::set<std::string> Crawler::extractLinks(std::string response, std::string url)
{
    utils::TraceTimer timer(url.c_str());
    utils::ScopedTimer st(__func__, __func__);
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

std::set<std::string> Crawler::crawl(std::set<std::string> initialRequests)
{
    std::set<std::string> res;

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
            std::set<std::string> crawledLinks = future.get();
            res.insert(crawledLinks.cbegin(), crawledLinks.cend());
        }
    }
    return res;
}

} // namespace webcrawler
