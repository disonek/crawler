#include "Crawler.hpp"

#include <libxml/HTMLparser.h>
#include <libxml/uri.h>
#include <libxml/xpath.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "Threadpool.hpp"
#include "Url.hpp"
#include "Webcurl.hpp"

namespace webcrawler {
Crawler::Crawler(size_t numThreads)
    : pool(new ThreadPool(numThreads, urlsInPool))
{
}

Crawler::~Crawler()
{
    stopped = true;
    urlsInPool.notify_all();
}

void Crawler::start(const std::string& startURL)
{
    pool->enqueue([&] {
        // task
        crawl(startURL);
    });
    while(!stopped)
    {
        std::unique_lock<std::mutex> poolLock(url_mut);
        // check if the links pool is empty, if it is, wait for condition_variable, until it's not
        do
        {
            urlsInPool.wait(poolLock);
        } while(urlPool.empty() && !stopped);
        if(stopped)
        {
            poolLock.unlock();
            return;
        }
        // get amount of urls equal to amount of free workers and amount of links
        size_t workersFree = pool->getAmountFreeWorkers();
        // spdlog::info("Workers free: {}", workersFree);
        for(size_t i = 0; i < workersFree && i < urlPool.size(); i++)
        {
            // get an url to crawl
            std::string nextURL = urlPool.front();
            urlPool.pop();
            // crawl the next url
            pool->enqueue([&] {
                // task
                crawl(nextURL);
            });
        }
    }
}

void Crawler::stop()
{
    stopped = true;
}

std::vector<std::string> Crawler::extractLinks(std::string response, std::string url)
{
    std::vector<std::string> foundLinks;
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
            foundLinks.push_back(std::string{foundLink});
        xmlFree(foundLink);
    }
    xmlXPathFreeObject(result);
    xmlFreeDoc(doc);
    return foundLinks;
}

void Crawler::crawl(const std::string& url)
{
    spdlog::info("Crawling {}", url);
    std::string pageContent;
    try
    {
        pageContent = WebCurl::getPage(url);
        // spdlog::info(": pageContent {}", pageContent);
    }
    catch(const std::runtime_error& err)
    {
        // spdlog::info("AN ERROR OCCURRED: {} {}", err.what(), url);
        return; // change this is the future
    }

    std::vector<std::string> links = extractLinks(pageContent, url);

    // for(const std::string& link : links)
    // {
    //     spdlog::info("{}", link);
    // }

    for(const std::string& link : links)
    {
        // std::string query;
        std::lock_guard<std::mutex> foundLock(found_mut);
        if(foundURLs.find(link) == foundURLs.end())
        {
            // add the url to foundurls, so the crawler won't download the page again
            std::lock_guard<std::mutex> poolLock(url_mut);
            foundURLs.insert(link);
            urlPool.push(link);
            // query = "INSERT INTO links VALUES('"+ link +"')";
            // mysql_query(conn,query.c_str());
        }
    }
    // mysql_close(conn);
}
} // namespace webcrawler
