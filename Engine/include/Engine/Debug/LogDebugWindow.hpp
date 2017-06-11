/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Debug/Logger.hpp>
#include <Engine/Debug/DebugWindow.hpp>

#define LIMIT_WIDTH 600

class LogDebugWindow: public DebugWindow
{
public:
    LogDebugWindow(std::shared_ptr<Logger> logger);
    LogDebugWindow(std::shared_ptr<Logger> logger, const glm::vec2& pos, const glm::vec2& size);
    virtual ~LogDebugWindow();

    void        retrieveLogColors();
    void        build(std::shared_ptr<GameState> gameState, float elapsedTime) override final;

    GENERATE_ID(LogDebugWindow);

private:
    std::shared_ptr<Logger> _logger;
    uint32_t                _lastLogSize;
    ImColor*                _logColors;
};
