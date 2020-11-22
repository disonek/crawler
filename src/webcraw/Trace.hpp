#pragma once

#include <spdlog/spdlog.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

namespace utils {

struct ProfileResult
{
    std::string Name;
    std::chrono::duration<double, std::micro> Start;
    std::chrono::microseconds ElapsedTime;
    std::thread::id ThreadID;
};

struct InstrumentationSession
{
    std::string Name;
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name);
    ~InstrumentationTimer();
    void Stop();

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool m_Stopped;
};

class Instrumentor
{
public:
    Instrumentor(const Instrumentor&) = delete;
    Instrumentor(Instrumentor&&) = delete;

    static void session(const std::string& name, const std::string& filepath = "results.json")
    {
        Instrumentor::Get().BeginSession(name, filepath);
    }

    static void endSession()
    {
        Instrumentor::Get().EndSession();
    }
    static void scope(std::string name)
    {
        InstrumentationTimer timer(name.c_str());
    }

    void BeginSession(const std::string& name, const std::string& filepath = "results.json");
    void EndSession();
    void WriteProfile(const ProfileResult& result);
    static Instrumentor& Get();

private:
    Instrumentor();
    ~Instrumentor();
    void WriteHeader();
    void WriteFooter();
    void InternalEndSession();

private:
    std::mutex m_Mutex;
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
};

} // namespace utils

namespace InstrumentorUtils {

template <size_t N>
struct ChangeResult
{
    char Data[N];
};

template <size_t N, size_t K>
constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
{
    ChangeResult<N> result = {};

    size_t srcIndex = 0;
    size_t dstIndex = 0;
    while(srcIndex < N)
    {
        size_t matchIndex = 0;
        while(matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
            matchIndex++;
        if(matchIndex == K - 1)
            srcIndex += matchIndex;
        result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
        srcIndex++;
    }
    return result;
}
} // namespace InstrumentorUtils
