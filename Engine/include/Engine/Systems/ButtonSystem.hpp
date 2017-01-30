/**
* @Author   Guillaume Labey
*/

#pragma once

#include <ECS/System.hpp>

#include <Engine/Core/GameStateManager.hpp>
#include <Engine/Components.hh>

START_SYSTEM(ButtonSystem)
public:
    ButtonSystem(GameStateManager* gameStateManager);
    virtual ~ButtonSystem();

    virtual bool                        init();
    virtual void                        update(EntityManager &em, float elapsedTime);

private:
    void                                removeSelected(Entity* entity);
    void                                setupSelectedIcon();

    void                                handleButtonMouseHover(EntityManager& em, Entity* entity, uint32_t entityIdx, const glm::vec2& cursorPos);
    void                                handleButtonsKeys(EntityManager& em);
    void                                handleButtonsActions(EntityManager& em);

    void                                setSelected(EntityManager &em, int buttonIdx, bool hovered = false);
    void                                setSelected(Entity* entity, bool hovered = false);
    void                                removeSelected(EntityManager &em, int buttonIdx);


private:
    GameStateManager*                   _gameStateManager;

    int                                 _currentSelected;
    bool                                _buttonHovered;

    Entity*                             _iconSelected = nullptr;
    sRenderComponent*                   _iconRender = nullptr;
END_SYSTEM(ButtonSystem)
