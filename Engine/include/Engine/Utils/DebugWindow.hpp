#pragma once

#include <string>
#include <glm/vec2.hpp>

class DebugWindow
{
public:
    DebugWindow(const std::string& tile = "", const glm::vec2& pos = { 0, 0}, const glm::vec2& size = { 100, 100} );
    virtual     ~DebugWindow();

    virtual void        build() = 0;

    const std::string&  getTitle() const;
    void                setTitle(const std::string& title);

    bool                isDisplayed() const;
    void                setDisplayed(bool displayed);

protected:
    std::string         _title;
    glm::vec2           _pos;
    glm::vec2           _size;
    bool                _displayed;
};
