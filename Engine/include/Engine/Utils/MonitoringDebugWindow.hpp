#pragma once

#include <memory>
#include <vector>
#include <map>
#include <imgui.h>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/DebugWindow.hpp>

#define ENABLE_COLOR            true

#define ADD_MONITORING_VAR      \
                                private: \
                                    uint16_t        _monitoringKey;

#define ABS(x)                  ((x) < 0 ? -(x) : (x))
#define TIME_DIFF_RATIO         (0.03)

typedef struct sMonitoring
{
    std::string         name;
    float               timeSec;
    float               avgTimeSec;
    float               oldAvg;
    uint32_t            nbEntities;
    std::vector<float>  timeLogs;
}                       tMonitoring;

class MonitoringDebugWindow : public DebugWindow
{
public:
    MonitoringDebugWindow(const glm::vec2& pos = glm::vec2(0, 650), const glm::vec2& size = glm::vec2(450, 300));
    virtual ~MonitoringDebugWindow();

    static std::shared_ptr<MonitoringDebugWindow>   getInstance();

    virtual void                                    build(float elapsedTime);

    uint16_t                                        registerSystem(std::string name);
    void                                            updateSystem(uint16_t key, float timeSec, uint32_t nbEntities);

private:
    float                                           calcTimeAverage(std::vector<float> timeLogs);
    void                                            updateTimeLogsSystem(tMonitoring& system, bool *resetCheckSec);
    ImColor                                         getDisplayColor(tMonitoring& system);
    void                                            displaySystem(tMonitoring& system);

private:
    static std::shared_ptr<MonitoringDebugWindow>   _monitoringDebugWindow;

    std::map<uint16_t, tMonitoring>                 _systemsRegistered;
    float                                           _checkSec;
};

