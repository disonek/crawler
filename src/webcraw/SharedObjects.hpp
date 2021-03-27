#pragma once

#include <atomic>
#include <future>
#include <mutex>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <utility>

template <typename T>
class TemplatedProtectedQueue
{
public:
    void push(T item)
    {
        std::lock_guard<std::mutex> lk(mutex);
        queue.push(item);
    }

    std::optional<T> pop()
    {
        std::lock_guard<std::mutex> lk(mutex);
        if(!queue.empty())
        {
            auto result = queue.front();
            queue.pop();
            return result;
        }
        return std::nullopt;
    }

private:
    std::mutex mutex;
    std::queue<T> queue;
};

class ProtectedQueue
{
public:
    using Response = std::set<std::string>;
    using Request = std::string;

    void pushResponse(Response response)
    {
        responses.push(response);
    }

    std::optional<Response> popResponse()
    {
        return responses.pop();
    }

    void pushRequest(Request request)
    {
        requests.push(request);
    }

    std::optional<Request> popRequest()
    {
        return requests.pop();
    }

    std::atomic<bool> ready{false};

private:
    std::mutex readyMutex;

    TemplatedProtectedQueue<Response> responses;
    TemplatedProtectedQueue<Request> requests;
};
