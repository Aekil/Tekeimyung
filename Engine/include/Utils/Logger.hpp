#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <imgui.h>

#include "Utils/Debug.hpp"


template<typename... Args>
std::string formatLogMessage(const char* format, Args... args)
{
    std::string buffer;
    buffer.resize(512);
    int size = snprintf(const_cast<char*>(buffer.c_str()), 512, format, args...);

    ASSERT(size >= 0, "The formated message should correctly be copied in the buffer");

    buffer.resize(size);
    return buffer;
}


#define LOG_TRACE(format, ...) Logger::getInstance()->log(Logger::eLogLevel::TRACE, formatLogMessage(format, ## __VA_ARGS__))
#define LOG_DEBUG(format, ...) Logger::getInstance()->log(Logger::eLogLevel::DEBUG, formatLogMessage(format, ## __VA_ARGS__))
#define LOG_INFO(format, ...)  Logger::getInstance()->log(Logger::eLogLevel::INFO, formatLogMessage(format, ## __VA_ARGS__))
#define LOG_WARN(format, ...)  Logger::getInstance()->log(Logger::eLogLevel::WARN, formatLogMessage(format, ## __VA_ARGS__))
#define LOG_ERROR(format, ...) Logger::getInstance()->log(Logger::eLogLevel::ERROR, formatLogMessage(format, ## __VA_ARGS__))

class           Logger
{
public:
    enum class  eLogLevel
    {
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };
public:
    explicit    Logger();
    virtual     ~Logger() {}

    bool        initialize();
    void        shutdown();

    static std::shared_ptr<Logger>  getInstance();
    std::ofstream&                  getStream();

    std::string              getLevelToString(Logger::eLogLevel level);
    std::string              getDateToString();

    void log(Logger::eLogLevel level, const std::string& message);

    const ImGuiTextBuffer&  getLog() const;
private:
    std::ofstream   _stream;

    static std::shared_ptr<Logger>  _instance;

     ImGuiTextBuffer     _log;
};
