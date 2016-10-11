#include <ctime>
#include <iostream>

#include <Utils/Logger.hpp>

std::shared_ptr<Logger> Logger::_instance;

Logger::Logger() {}

bool    Logger::initialize()
{
    time_t      time;
    struct tm*  now;

    _stream.open("engine.log", std::ios::out | std::ios::app);
    if (!_stream.is_open())
    {
        std::cerr << "Could not open the Logger properly." << std::endl;
        return (false);
    }
    time = std::time(0);
    now = std::localtime(&time);
    _stream << std::asctime(now) << std::endl;
    return (true);
}

void    Logger::shutdown()
{
    _stream.close();
}

std::shared_ptr<Logger> Logger::getInstance()
{
    return (_instance);
}

std::ofstream&  Logger::getStream()
{
    return (_stream);
}

void    Logger::setInstance(std::shared_ptr<Logger> instance)
{
    _instance = instance;
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

void    Logger::log(Logger::eLogLevel level, std::string message)
{
    std::ofstream&  stream = Logger::getInstance()->getStream();

    if (stream.is_open() && stream.good())
    {
        stream << "[" <<
            Logger::getLevelToString(level) <<
            "]\t" << message << std::endl;
    }
}