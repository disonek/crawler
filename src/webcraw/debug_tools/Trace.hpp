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

class Trace
{
public:
    using TraceSesion = std::string;
    Trace()
        : currentSesion(nullptr)
        , profileCount(0)
    {
    }

    void beginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        outputStream.open(filepath);
        writeHeader();
        currentSesion = new TraceSesion{name};
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

    static Trace& get()
    {
        static Trace instance;
        return instance;
    }

private:
    TraceSesion* currentSesion;
    std::ofstream outputStream;
    int profileCount;
};

} // namespace utils