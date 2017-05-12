/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Core/Components/RenderComponent.hh>
#include <Engine/Core/GameStateManager.hpp>

START_SYSTEM(ButtonSystem)
public:
    ButtonSystem(GameStateManager* gameStateManager);
    virtual ~ButtonSystem();

    void                                setupSelectedIcon();
    virtual void                        update(EntityManager &em, float elapsedTime);

private:
    void                                removeSelected(Entity* entity);

    void                                handleButtonMouseHover(EntityManager& em, Entity* entity, uint32_t entityIdx, const glm::vec2& cursorPos);
    void                                handleButtonsKeys(EntityManager& em);
    void                                handleButtonsActions(EntityManager& em);

    void                                setSelected(EntityManager &em, int buttonIdx, bool hovered = false);
    void                                setSelected(Entity* entity, bool hovered = false);
    void                                removeSelected(EntityManager &em, int buttonIdx);

    uint32_t                            getNextButtonEnabled(EntityManager& em, uint32_t buttonIdx, uint32_t recurse = 0);
    uint32_t                            getPreviousButtonEnabled(EntityManager& em, uint32_t buttonIdx, uint32_t recurse = 0);


private:
    GameStateManager*                   _gameStateManager;

    int                                 _currentSelected;
    bool                                _buttonHovered;

    Entity*                             _iconSelected = nullptr;
    sRenderComponent*                   _iconRender = nullptr;
END_SYSTEM(ButtonSystem)
