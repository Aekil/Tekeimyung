#include <ctime>
#include <iomanip>
#include <iostream>

#include <Utils/Logger.hpp>

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
    _stream << "Current date is: " << Logger::getDateToString() << std::endl;
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
    time_t      time;
    struct tm   now;
    char        format[26];

    time = std::time(0);
    localtime_s(&now, &time);
    asctime_s(format, sizeof(format), &now);
    format[sizeof(format) - 2] = '\0';
    return (format);
}

void    Logger::log(Logger::eLogLevel level, const std::string& message)
{
    std::ofstream&  stream = getStream();
    if (stream.is_open() && stream.good())
    {
        stream << "[" <<
            getDateToString() <<
            " - " << getLevelToString(level) <<
            "]\t" << message << std::endl;
    }

    _log.append("%s\n", message.c_str());
}

const ImGuiTextBuffer&  Logger::getLog() const
{
    return _log;
}
