/**
* @Author   Mathieu Chassara
*/

// Define this flag before time.h to use secure versions of localtime and asctime on Windows
#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <iomanip>
#include <iostream>

#include <Engine/Debug/Logger.hpp>

std::shared_ptr<Logger> Logger::_instance;

Logger::Logger() {}

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

Logger::eLogLevel   Logger::getLevelByIndex(int index)
{
    Logger::eLogLevel   level;

    level = (index >= 5 ? Logger::eLogLevel::UNKNOWN : static_cast<Logger::eLogLevel>(index));
    return (level);
}

std::string Logger::getLevelToString(Logger::eLogLevel level)
{
    std::string value;

    switch (level)
    {
        case Logger::eLogLevel::TRACE: value = "TRACE"; break;
        case Logger::eLogLevel::DEBUG: value = "DEBUG"; break;
        case Logger::eLogLevel::INFO: value = "INFO"; break;
        case Logger::eLogLevel::WARN: value = "WARN"; break;
        case Logger::eLogLevel::ERROR: value = "ERROR"; break;
        default: value = "???"; break;
    }
    return (value);
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
            " - " << getLevelToString(level) <<
            "]\t" << message << std::endl;
    }
    
#if defined(ENGINE_DEBUG)
    _log.append("[%s]\t%s\n", getLevelToString(level).c_str(), message.c_str());
#endif
}

const ImGuiTextBuffer&  Logger::getLog() const
{
    return (_log);
}
