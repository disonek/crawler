#include <iostream>
#include <exception>

#include "Crawler.hpp"
#include "SharedObjects.hpp"
#include "imgui/ImGuiLayer.hpp"

// Helper function to safely wait for async thread and handle exceptions
template<typename T>
bool safeThreadWait(std::future<T>& future, ProtectedQueue& taskQueue, const std::string& threadName)
{
    try
    {
        future.get();
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception in " << threadName << " thread: " << e.what() << std::endl;
        taskQueue.ready.store(true); // Signal graceful shutdown to other thread
        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown exception in " << threadName << " thread" << std::endl;
        taskQueue.ready.store(true); // Signal graceful shutdown to other thread
        return false;
    }
}

void runByAsync()
{
    try
    {
        ProtectedQueue taskQueue;

        webcrawler::Crawler crawler;
        img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};

        auto guiResult = std::async(std::launch::async, [&taskQueue, &imGuiLayer] {
            imGuiLayer.intialize();
            imGuiLayer.run(taskQueue);
        });

        auto crawlerResult = std::async(std::launch::async, [&taskQueue, &crawler] { crawler.run(taskQueue); });

        bool guiSuccess = safeThreadWait(guiResult, taskQueue, "GUI");
        bool crawlerSuccess = safeThreadWait(crawlerResult, taskQueue, "Crawler");

        if(!guiSuccess || !crawlerSuccess)
        {
            std::cerr << "One or more threads failed during execution" << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Unexpected exception in runByAsync: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown exception in runByAsync" << std::endl;
    }
}

void runByThreads()
{
    try
    {
        ProtectedQueue taskQueue;

        webcrawler::Crawler crawler;
        img::ImGuiLayer imGuiLayer{std::make_shared<img::OpenGLModule>(), std::make_shared<img::ImGuiLogger>()};
        
        std::exception_ptr crawlerException;
        std::exception_ptr guiException;

        std::thread crawlerThread([&crawler, &taskQueue, &crawlerException] {
            try
            {
                crawler.run(taskQueue);
            }
            catch(...)
            {
                crawlerException = std::current_exception();
                taskQueue.ready.store(true); // Signal graceful shutdown
            }
        });

        std::thread imGuiLayerThread([&taskQueue, &imGuiLayer, &guiException]() {
            try
            {
                imGuiLayer.intialize();
                imGuiLayer.run(taskQueue);
            }
            catch(...)
            {
                guiException = std::current_exception();
                taskQueue.ready.store(true); // Signal graceful shutdown
            }
        });

        crawlerThread.join();
        imGuiLayerThread.join();

        // Rethrow any exceptions that occurred
        if(crawlerException)
        {
            std::cerr << "Exception occurred in Crawler thread" << std::endl;
            std::rethrow_exception(crawlerException);
        }
        if(guiException)
        {
            std::cerr << "Exception occurred in ImGuiLayer thread" << std::endl;
            std::rethrow_exception(guiException);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception in runByThreads: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown exception in runByThreads" << std::endl;
    }
}

void runInTerminalModeTryout()
{
    ProtectedQueue taskQueue;
    taskQueue.pushRequest("https://www.google.com/doodles");
    webcrawler::Crawler crawler;
    auto crawlerResult = std::async(std::launch::async, [&taskQueue, &crawler] { crawler.run(taskQueue); });
    crawlerResult.get();
    auto response = taskQueue.popResponse();

    if(std::nullopt != response)
    for(auto message : response.value())
    {
        std::cout << message << "\n";
    }

}


int main(int argc, char* argv[])
{
    runByAsync();
}
