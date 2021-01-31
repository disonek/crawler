#pragma once

#include <future>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <utility>

template <typename Response = std::set<std::string>>
class BasicProtectedQueue
{
public:
    using Request = std::string;
    void pushResponse(Response response)
    {
        std::lock_guard<std::mutex> lk(responsesMutex);
        responses.push(response);
    }

    std::optional<Response> popResponse()
    {
        std::lock_guard<std::mutex> lk(responsesMutex);
        if(!responses.empty())
        {
            auto result = responses.front();
            responses.pop();
            return result;
        }
        return std::nullopt;
    }

    void pushRequest(Request request)
    {
        std::lock_guard<std::mutex> lk(requestsMutex);
        requests.push(request);
    }

    std::optional<Request> popRequest()
    {
        std::lock_guard<std::mutex> lk(requestsMutex);
        if(!requests.empty())
        {
            auto result = requests.front();
            requests.pop();
            return result;
        }
        return std::nullopt;
    }

private:
    std::mutex responsesMutex;
    std::mutex requestsMutex;
    std::queue<Response> responses;
    std::queue<std::string> requests;
};
