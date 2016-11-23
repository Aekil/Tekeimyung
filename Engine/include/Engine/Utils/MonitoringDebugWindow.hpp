#pragma once

#include <vector>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/DebugWindow.hpp>

template<typename... Args>
std::string formatMonMessage(const char* format, Args... args)
{
    std::string buffer;
    buffer.resize(512);
    int size = snprintf(const_cast<char*>(buffer.c_str()), 512, format, args...);

    ASSERT(size >= 0, "The formated message should correctly be copied in the buffer");

    buffer.resize(size);
    return buffer;
}

#define FMT_MSG(format, ...)  (formatMonMessage(format, ## __VA_ARGS__))

class MonitoringDebugWindow : public DebugWindow
{
public:
    MonitoringDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~MonitoringDebugWindow();

    virtual void    build();

    void            registerMsg(std::string msg);

private:
    std::vector<std::string>    _msgList;
};

