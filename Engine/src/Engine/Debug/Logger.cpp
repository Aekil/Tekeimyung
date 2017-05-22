/**
* @Author   Mathieu Chassara
*/

// Define this flag before time.h to use secure versions of localtime and asctime on Windows
#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <Engine/Debug/Logger.hpp>

std::shared_ptr<Logger> Logger::_instance;

Logger::Logger()
{
    // TODO: Log only ERROR and WARN when LogDebugWindow filter work
    _logLevel =  eLogLevel::DEBUG | eLogLevel::INFO | eLogLevel::TRACE | eLogLevel::ERROR | eLogLevel::WARN;
}

bool    Logger::initialize()
{
    _stream.open("engine.log", std::ios::out | std::ios::app);
    if (!_stream.is_open())
    {
        std::cerr << "Could not open the log file properly." << std::endl;
        return (false);
    }
    _stream << "===========================================" << std::endl;
    _stream << "A new instance of Logger has been created !" << std::endl;
    _stream << "Current date is: " << getDateToString() << std::endl;
    _stream << "===========================================" << std::endl;
    return (true);
}

void    Logger::shutdown()
{
    _stream << "===========================================" << std::endl;
    _stream << "Closing the current instance of Logger ...." << std::endl;
    _stream << "===========================================" << std::endl;
    _stream.close();
}

std::shared_ptr<Logger> Logger::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<Logger>();

    return (_instance);
}

std::ofstream&  Logger::getStream()
{
    return (_stream);
}

std::string Logger::getDateToString()
{
    time_t      rawTime;
    struct tm   timeInfo;
    char        format[ASCTIME_BUFFER_SIZE];

    std::time(&rawTime);
    std::memset(format, 0, ASCTIME_BUFFER_SIZE);

    #if defined(_WIN32)
    // Use windows secure versions of localtime and asctime
        localtime_s(&timeInfo, &rawTime);
        asctime_s(format, sizeof(format), &timeInfo);
    #else
    // Use linux secure versions of localtime and asctime
        localtime_r(&rawTime, &timeInfo);
        asctime_r(&timeInfo, format);
    #endif
    format[ASCTIME_BUFFER_SIZE - 2] = '\0';                 // Removing the newline character at the end, here.
    return (format);
}

void    Logger::log(Logger::eLogLevel level, const std::string& message)
{
    if (_stream.is_open() && _stream.good())
    {
        _stream << "[" <<
            getDateToString() <<
            " - " << EnumManager<Logger::eLogLevel>::enumToString(level) <<
            "]\t" << message << std::endl;
    }

#if defined(ENGINE_DEBUG)
    std::stringstream log;
    log << "[" <<
        EnumManager<Logger::eLogLevel>::enumToString(level) <<
        "]\t" << message;
    sLogInfo logInfo = {log.str(), level};
    _logs.push_back(logInfo);
    addConsoleLog(logInfo);
#endif
}

const Logger::sConsoleLog&  Logger::getConsoleLog() const
{
    return (_log);
}

void    Logger::setLogLevel(Logger::eLogLevel logLevel)
{
    _logLevel = logLevel;

#if defined(ENGINE_DEBUG)
    _log.buf.clear();
    for (sLogInfo& logInfo: _logs)
    {
        addConsoleLog(logInfo);
    }
#endif
}

Logger::eLogLevel   Logger::getLogLevel() const
{
    return (_logLevel);
}

void    Logger::addConsoleLog(const sLogInfo& logInfo)
{
    if (!(_logLevel & logInfo.level))
    {
        return;
    }

     int oldSize = _log.buf.size();
    _log.buf.append("%s\n", logInfo.message.c_str());
    for (int newSize =  _log.buf.size(); oldSize < newSize; oldSize++)
    {
        if (_log.buf[oldSize] == '\n')
        {
            _log.lineOffsets.push_back(oldSize);
        }
    }
}
