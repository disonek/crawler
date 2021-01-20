#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace mess {

struct MessageBase
{
    virtual ~MessageBase() = default;
};

template <typename Msg>
struct WrappedMessage : MessageBase
{
    explicit WrappedMessage(Msg const& contents)
        : contents(contents)
    {
    }

    Msg contents;
};

class Queue
{
public:
    template <typename T>
    void push(T const& msg)
    {
        std::lock_guard<std::mutex> lk(mutex);
        internalQueue.push(std::make_shared<WrappedMessage<T>>(msg));
        condition.notify_all();
    }

    std::shared_ptr<MessageBase> waitAndPop()
    {
        std::unique_lock<std::mutex> lk(mutex);
        condition.wait(lk, [&] { return !internalQueue.empty(); });
        auto res = internalQueue.front();
        internalQueue.pop();
        return res;
    }

private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<std::shared_ptr<MessageBase>> internalQueue;
};

class Sender
{
public:
    explicit Sender(Queue* queue)
        : queue(queue)
    {
    }

    template <typename Message>
    void send(Message const& msg)
    {
        if(queue)
        {
            queue->push(msg);
        }
    }

private:
    Queue* queue;
};

template <typename PreviousDispatcher, typename Msg, typename Func>
class TemplatedDispatcher
{
public:
    TemplatedDispatcher(TemplatedDispatcher&& other)
        : queue(other.queue)
        , previousDispatcher(other.previousDispatcher)
        , func(std::move(other.func))
        , chained(other.chained)
    {
        other.chained = true;
    }

    TemplatedDispatcher(Queue* queue, PreviousDispatcher* prev, Func&& func)
        : queue(queue)
        , previousDispatcher(prev)
        , func(std::forward<Func>(func))
        , chained{false}
    {
        prev->chained = true;
    }

    template <typename OtherMsg, typename OtherFunc>
    TemplatedDispatcher<TemplatedDispatcher, OtherMsg, OtherFunc> handle(OtherFunc&& otherFunc)
    {
        return TemplatedDispatcher<TemplatedDispatcher, OtherMsg, OtherFunc>(
            queue, this, std::forward<OtherFunc>(otherFunc));
    }

    ~TemplatedDispatcher() noexcept(false)
    {
        if(!chained)
        {
            waitAndDispatch();
        }
    }

private:
    Queue* queue;
    PreviousDispatcher* previousDispatcher;
    Func func;
    bool chained;

    TemplatedDispatcher(TemplatedDispatcher const&) = delete;
    TemplatedDispatcher& operator=(TemplatedDispatcher const&) = delete;

    template <typename Dispatcher, typename OtherMsg, typename OtherFunc>
    friend class TemplatedDispatcher;

    void waitAndDispatch()
    {
        for(;;)
        {
            auto msg = queue->waitAndPop();
            if(dispatch(msg))
                break;
        }
    }

    bool dispatch(std::shared_ptr<MessageBase> const& msg)
    {
        if(WrappedMessage<Msg>* wrapper = dynamic_cast<WrappedMessage<Msg>*>(msg.get()))
        {
            func(wrapper->contents);
            return true;
        }
        else
        {
            return previousDispatcher->dispatch(msg);
        }
    }
};

class CloseQueue
{
};

class Dispatcher
{
public:
    Dispatcher(Dispatcher&& other)
        : queue(other.queue)
        , chained(other.chained)
    {
        other.chained = true;
    }

    explicit Dispatcher(Queue* queue)
        : queue{queue}
        , chained{false}
    {
    }

    template <typename Message, typename Func>
    TemplatedDispatcher<Dispatcher, Message, Func> handle(Func&& func)
    {
        return TemplatedDispatcher<Dispatcher, Message, Func>(queue, this, std::forward<Func>(func));
    }
    ~Dispatcher() noexcept(false)
    {
        if(!chained)
        {
            waitAndDispatch();
        }
    }

private:
    Dispatcher(Dispatcher const&) = delete;
    Dispatcher& operator=(Dispatcher const&) = delete;
    template <typename Dispatcher, typename Msg, typename Func>
    friend class TemplatedDispatcher;

    void waitAndDispatch()
    {
        for(;;)
        {
            auto msg = queue->waitAndPop();
            dispatch(msg);
        }
    }

    bool dispatch(std::shared_ptr<MessageBase> const& msg)
    {
        if(dynamic_cast<WrappedMessage<CloseQueue>*>(msg.get()))
        {
            throw CloseQueue();
        }
        return false;
    }
    Queue* queue;
    bool chained;
};

class Receiver
{
public:
    operator Sender()
    {
        return Sender(&queue);
    }

    Dispatcher wait()
    {
        return Dispatcher(&queue);
    }

private:
    Queue queue;
};
} // namespace mess