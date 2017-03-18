/**
* @Author   Mathieu Chassara
*/

#include <Engine/Window/Cursor.hpp>

Cursor::Cursor() : _entered(false), _xPos(0.0), _yPos(0.0) {}

bool    Cursor::isInTheWindow() const
{
    return (_entered);
}

double  Cursor::getX() const
{
    return (_xPos);
}

double  Cursor::getY() const
{
    return (_yPos);
}

void    Cursor::setWindowEntering(bool entering)
{
    _entered = entering;
}

void    Cursor::setXPosition(double xPos)
{
    _xPos = xPos;
}

void    Cursor::setYPosition(double yPos)
{
    _yPos = yPos;
}