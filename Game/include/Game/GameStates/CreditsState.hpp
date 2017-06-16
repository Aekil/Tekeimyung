/**
* @Author   Mathieu Chassara
*/

#pragma once

#include <Engine/Core/GameState.hpp>

#include <ECS/System.hpp>
#include <ECS/World.hpp>

#define MAX_SIZE_TEXT_PAGES (32)

START_GAMESTATE(CreditsState, "CreditsScreen")
 public:
     ~CreditsState();

     void    onEnter() override final;
     void    setupSystems() override final;
     bool    init() override final;
     bool    update(float elapsedTime) override final;

private:
    void    retrievePages();
    void    retrieveButtons();
    bool    handleButtons();

    void    updatePaging();
    void    updatePagingText();
    void    updateLevelDisplayed();

    void    setButton(Entity* buttonEntity, bool enabled);
private:
    std::vector<std::string>    _pages;
    int                         _currentPageIndex = 0;

    Entity*                     _buttonLeftChevron;
    Entity*                     _buttonRightChevron;

    int                         _tmpCurrentPageIndex = -1;
END_GAMESTATE(CreditsState)