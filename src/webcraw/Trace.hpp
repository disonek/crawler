//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::get().beginSession("Session Name");        // Begin session
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in
//     profiling
//     // Code
// }
// Instrumentor::get().endSession();                        // end Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile
// name.
//
#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <thread>

namespace utils {
struct ProfileResult
{
    std::string name;
    long long start, end;
    uint32_t threadID;
};

struct InstrumentationSession
{
    std::string name;
};

class Instrumentor
{
public:
    Instrumentor()
        : currentSesion(nullptr)
        , profileCount(0)
    {
    }

    void beginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        outputStream.open(filepath);
        writeHeader();
        currentSesion = new InstrumentationSession{name};
    }

    void endSession()
    {
        writeFooter();
        outputStream.close();
        delete currentSesion;
        currentSesion = nullptr;
        profileCount = 0;
    }

    void writeProfile(const ProfileResult& result)
    {
        if(profileCount++ > 0)
            outputStream << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        outputStream << "{";
        outputStream << "\"cat\":\"function\",";
        outputStream << "\"dur\":" << (result.end - result.start) << ',';
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":0,";
        outputStream << "\"tid\":" << result.threadID << ",";
        outputStream << "\"ts\":" << result.start;
        outputStream << "}";

        outputStream.flush();
    }

    void writeHeader()
    {
        outputStream << "{\"otherData\": {},\"traceEvents\":[";
        outputStream.flush();
    }

    void writeFooter()
    {
        outputStream << "]}";
        outputStream.flush();
    }

    static Instrumentor& get()
    {
        static Instrumentor instance;
        return instance;
    }

private:
    InstrumentationSession* currentSesion;
    std::ofstream outputStream;
    int profileCount;
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : name(name)
        , stopped(false)
    {
        startTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if(!stopped)
            stop();
    }

    void stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start =
            std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
        long long end =
            std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        uint32_t threadID = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
        Instrumentor::get().writeProfile({name, start, end, threadID});

        stopped = true;
    }

private:
    const char* name;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
    bool stopped;
};
} // namespace utils