#pragma once

#include <string>

class           DebugWindow
{
public:
    DebugWindow();
    virtual     ~DebugWindow();

    virtual void        build() = 0;

    const std::string&  getTitle() const;
    void                setTitle(const std::string& title);

    bool                isDisplayed() const;
    void                setDisplayed(bool displayed);

protected:
    std::string         _title;
    bool                _displayed;
};
