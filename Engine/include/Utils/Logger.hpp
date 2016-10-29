#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <imgui.h>

#define LOG_TRACE(message, ...) Logger::getInstance()->log(Logger::eLogLevel::TRACE, message, __VA_ARGS__)
#define LOG_DEBUG(message, ...) Logger::getInstance()->log(Logger::eLogLevel::DEBUG, message, __VA_ARGS__)
#define LOG_INFO(message, ...)  Logger::getInstance()->log(Logger::eLogLevel::INFO, message, __VA_ARGS__)
#define LOG_WARN(message, ...)  Logger::getInstance()->log(Logger::eLogLevel::WARN, message, __VA_ARGS__)
#define LOG_ERROR(message, ...) Logger::getInstance()->log(Logger::eLogLevel::ERROR, message, __VA_ARGS__)

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

    void log(Logger::eLogLevel level, std::string message, ...);

    const ImGuiTextBuffer&  getLog() const;
private:
    std::ofstream   _stream;

    static std::shared_ptr<Logger>  _instance;

     ImGuiTextBuffer     _log;
};
