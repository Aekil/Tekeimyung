/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/DebugWindow.hpp>

#define LIMIT_WIDTH 600

class LogDebugWindow: public DebugWindow
{
public:
    LogDebugWindow(std::shared_ptr<Logger> logger, const glm::vec2& pos, const glm::vec2& size);
    virtual ~LogDebugWindow();

    virtual void        build(float elapsedTime);

    GENERATE_ID(LogDebugWindow);

private:
    std::shared_ptr<Logger> _logger;
    uint32_t                _lastLogSize;
};
