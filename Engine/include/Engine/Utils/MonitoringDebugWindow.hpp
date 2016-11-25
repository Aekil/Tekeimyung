#pragma once

#include <memory>
#include <vector>
#include <map>

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

typedef struct sMonitoring
{
    std::string name;
    float       timeMs;
}               tMonitoring;

class MonitoringDebugWindow : public DebugWindow
{
public:
    //MonitoringDebugWindow();
    MonitoringDebugWindow(const glm::vec2& pos = glm::vec2(0, 650), const glm::vec2& size = glm::vec2(300, 200));
    virtual ~MonitoringDebugWindow();

    static std::shared_ptr<MonitoringDebugWindow>   getInstance();

    virtual void                                    build();

    uint16_t                                        registerSystem(std::string name);
    void                                            updateSystem(uint16_t key, tMonitoring newData);

private:
    static std::shared_ptr<MonitoringDebugWindow>   _monitoringDebugWindow;

    std::map<uint16_t, tMonitoring>                 _systemsRegistered;

    //std::vector<std::string>                        _msgList;
};

