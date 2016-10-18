#pragma once

#include <fstream>
#include <memory>
#include <string>

#define LOG_TRACE(message, ...) Logger::log(Logger::eLogLevel::TRACE, message, __VA_ARGS__)
#define LOG_DEBUG(message, ...) Logger::log(Logger::eLogLevel::DEBUG, message, __VA_ARGS__)
#define LOG_INFO(message, ...)  Logger::log(Logger::eLogLevel::INFO, message, __VA_ARGS__)
#define LOG_WARN(message, ...)  Logger::log(Logger::eLogLevel::WARN, message, __VA_ARGS__)
#define LOG_ERROR(message, ...) Logger::log(Logger::eLogLevel::ERROR, message, __VA_ARGS__)

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

    static void                     setInstance(std::shared_ptr<Logger> instance);

    static std::string              getLevelToString(Logger::eLogLevel level);
    static std::string              getDateToString();

    static void log(Logger::eLogLevel level, std::string message, ...);
private:
    std::ofstream   _stream;

    static std::shared_ptr<Logger>  _instance;
};