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

#define LOG_LEVELS(PROCESS)                 \
    PROCESS(UNKNOWN, 0)                     \
    PROCESS(TRACE, 1 << 0)                  \
    PROCESS(DEBUG, 1 << 1)                  \
    PROCESS(INFO, 1 << 2)                   \
    PROCESS(WARN, 1 << 3)                   \
    PROCESS(ERROR, 1 << 4)                  \

class           Logger
{
public:
    REGISTER_ENUM(eLogLevel, int, LOG_LEVELS)

    struct sLogInfo
    {
        std::string message;
        eLogLevel level;
    };

    struct sConsoleLog
    {
        ImGuiTextBuffer buf;
        ImVector<int>   lineOffsets;
    };

public:
    explicit    Logger();
    virtual     ~Logger() {}

    bool        initialize();
    void        shutdown();

    static std::shared_ptr<Logger>  getInstance();
    std::ofstream&                  getStream();

    void                            log(Logger::eLogLevel level, const std::string& message);

    const sConsoleLog&              getConsoleLog() const;

    void                            setLogLevel(Logger::eLogLevel logLevel);
    Logger::eLogLevel               getLogLevel() const;

private:
    std::string                     getDateToString();
    void                            addConsoleLog(const sLogInfo& logInfo);

private:
    std::ofstream   _stream;

    static std::shared_ptr<Logger>  _instance;

    std::vector<sLogInfo>       _logs;
    sConsoleLog                 _log;

    Logger::eLogLevel           _logLevel;
};

REGISTER_ENUM_MANAGER(Logger::eLogLevel, LOG_LEVELS)

inline Logger::eLogLevel operator~(const Logger::eLogLevel& rhs) {
    return (static_cast<Logger::eLogLevel>(~static_cast<int>(rhs)));
}

inline Logger::eLogLevel operator|(const Logger::eLogLevel& lhs, const Logger::eLogLevel& rhs) {
    return (static_cast<Logger::eLogLevel>(static_cast<int>(lhs) | static_cast<int>(rhs)));
}

inline Logger::eLogLevel operator|=(const Logger::eLogLevel& lhs, const Logger::eLogLevel& rhs) {
    return (static_cast<Logger::eLogLevel>(static_cast<int>(lhs) | static_cast<int>(rhs)));
}

inline int operator&(const Logger::eLogLevel& lhs, const Logger::eLogLevel& rhs) {
    return (static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline Logger::eLogLevel operator&=(const Logger::eLogLevel& lhs, const Logger::eLogLevel& rhs) {
    return (static_cast<Logger::eLogLevel>(static_cast<int>(lhs) & static_cast<int>(rhs)));
}
