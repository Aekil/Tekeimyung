/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <string>
#include <glm/vec2.hpp>

#include <ECS/crc32.hh>
#include <Engine/Core/GameState.hpp>

class DebugWindow
{
public:
    DebugWindow(const std::string& tile = "", const ImVec2& pos = {0, 0}, const ImVec2& size = {192, 108});
    virtual     ~DebugWindow();

    static void         applyGlobalStyle();

    virtual void        build(std::shared_ptr<GameState> gameState, float elapsedTime) = 0;

    const std::string&  getTitle() const;
    void                setTitle(const std::string& title);

    bool                isDisplayed() const;
    void                isDisplayed(bool displayed);

    virtual uint32_t    getId() const = 0;

protected:
    void                setDisableButtonStyle();
    void                removeDisableButtonStyle();

protected:
    std::string         _title;
    ImVec2              _pos;
    ImVec2              _size;
    bool                _displayed;
};

#define GENERATE_ID(name)                                                       \
static constexpr unsigned int identifier = #name##_crc32;                       \
uint32_t            getId() const override final                                \
{                                                                               \
    return (name::identifier);                                                  \
}