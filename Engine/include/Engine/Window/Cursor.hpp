#pragma once

class           Cursor
{
public:
    explicit    Cursor();
    virtual     ~Cursor() {}

    bool        isInTheWindow() const;
    double      getX() const;
    double      getY() const;

    void        setWindowEntering(bool entering);
    void        setXPosition(double xPos);
    void        setYPosition(double yPos);
private:
    bool        _entered;
    double      _xPos, _yPos;
};