/**
* @Author   Guillaume Labey
*/

#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <ECS/crc32.hh>

class DebugWindow
{
public:
    DebugWindow(const std::string& tile = "", const glm::vec2& pos = {0, 0}, const glm::vec2& size = {100, 100});
    virtual     ~DebugWindow();

    virtual void        build(float elapsedTime) = 0;

    const std::string&  getTitle() const;
    void                setTitle(const std::string& title);

    bool                isDisplayed() const;
    void                setDisplayed(bool displayed);

    virtual uint32_t    getId() const = 0;

protected:
    void                setDisableButtonStyle();
    void                removeDisableButtonStyle();

protected:
    std::string         _title;
    glm::vec2           _pos;
    glm::vec2           _size;
    bool                _displayed;
};


#define GENERATE_ID(name)                                                       \
static constexpr unsigned int identifier = #name##_crc32;                       \
uint32_t            getId() const override final                                \
{                                                                               \
    return (name::identifier);                                                  \
}
