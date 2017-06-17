/**
* @Author   Julien Chardon
*/

#pragma once

#include <memory>
#include <vector>
#include <map>

#include <Engine/Debug/Debug.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Timer.hpp>
#include <Engine/Debug/DebugWindow.hpp>

#define ENABLE_COLOR            true

#define ABS(x)                  ((x) < 0 ? -(x) : (x))
#define TIME_DIFF_RATIO         (0.03)

typedef struct sMonitoring
{
    const char*         name;
    float               timeSec;
    float               avgTimeSec;
    float               oldAvg;
    uint32_t            nbEntities;
    std::vector<float>  timeLogs;
    uint8_t             dirty = 0; // This flag let us know if the data is initialized
}                       tMonitoring;

class MonitoringDebugWindow : public DebugWindow
{
public:
    MonitoringDebugWindow();
    MonitoringDebugWindow(const glm::vec2& pos, const glm::vec2& size);
    virtual ~MonitoringDebugWindow();

    static std::shared_ptr<MonitoringDebugWindow>   getInstance();

    void                                            build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    void                                            updateSystem(uint16_t key, float timeSec, uint32_t nbEntities, const char* name);

    GENERATE_ID(MonitoringDebugWindow);

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

