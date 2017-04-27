/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <imgui.h>

#include <Engine/Debug/Debug.hpp>
#include <Engine/Utils/Helper.hpp>

/**
    The textual representation of the calendar time given by the asctime function
    has a fixed 25-character form, that is why the buffer is set at 26.

    See the links below for more infos :
        - http://www.cplusplus.com/reference/ctime/asctime/
        - http://en.cppreference.com/w/cpp/chrono/c/asctime
*/
#define ASCTIME_BUFFER_SIZE (26)

#define LOG_TRACE(format, ...) Logger::getInstance()->log(Logger::eLogLevel::TRACE, Helper::formatMessage(format, ## __VA_ARGS__))
#define LOG_DEBUG(format, ...) Logger::getInstance()->log(Logger::eLogLevel::DEBUG, Helper::formatMessage(format, ## __VA_ARGS__))
#define LOG_INFO(format, ...)  Logger::getInstance()->log(Logger::eLogLevel::INFO, Helper::formatMessage(format, ## __VA_ARGS__))
#define LOG_WARN(format, ...)  Logger::getInstance()->log(Logger::eLogLevel::WARN, Helper::formatMessage(format, ## __VA_ARGS__))
#define LOG_ERROR(format, ...) Logger::getInstance()->log(Logger::eLogLevel::ERROR, Helper::formatMessage(format, ## __VA_ARGS__))

class           Logger
{
public:
    enum class  eLogLevel
    {
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        UNKNOWN,
        COUNT
    };

public:
    explicit    Logger();
    virtual     ~Logger() {}

    bool        initialize();
    void        shutdown();

    static std::shared_ptr<Logger>  getInstance();
    std::ofstream&                  getStream();

    Logger::eLogLevel       getLevelByIndex(int index);
    std::string             getLevelToString(Logger::eLogLevel level);
    std::string             getDateToString();

    void log(Logger::eLogLevel level, const std::string& message);

    const ImGuiTextBuffer&  getLog() const;

private:
    std::ofstream   _stream;

    static std::shared_ptr<Logger>  _instance;

     ImGuiTextBuffer     _log;
};
