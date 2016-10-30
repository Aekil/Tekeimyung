#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Utils/Exception.hpp"

#include "Core/GameState.hpp"

GameState::GameState() {}

GameState::~GameState() {}

bool    GameState::initSystems()
{
    for (auto system: _world.getSystems())
    {
        if (!system->init())
        {
            return (false);
        }
    }

    return (true);
}

bool    GameState::update(float elapsedTime)
{
    try
    {
        ImGui_ImplGlfwGL3_NewFrame();
        {
            // Update GameState debug windows
            for (auto&& debugWinwdow: _debugWindows)
            {
                if (debugWinwdow->isDisplayed())
                    debugWinwdow->build();
            }

            // Update GameState systems
            for (auto&& system: _world.getSystems())
            {
                system->update(_world.getEntityManager(), elapsedTime);
            }
        }
    }
    catch(const Exception &e)
    {
        std::cerr << e.what() << std::endl;
        return (false);
    }

    return (true);
}
