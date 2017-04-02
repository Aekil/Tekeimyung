/**
* @Author   Mathieu Chassara
*/

#include    <sstream>
#include    <string>

#include    <Engine/Utils/Logger.hpp>
#include    <Engine/Window/VideoMode.hpp>

VideoMode::VideoMode() {}

VideoMode::~VideoMode() {}

VideoMode::VideoMode(const GLFWvidmode* vidmode)
{
    this->_width = vidmode->width;
    this->_height = vidmode->height;
    this->_redBits = vidmode->redBits;
    this->_greenBits = vidmode->greenBits;
    this->_blueBits = vidmode->blueBits;
    this->_refreshRate = vidmode->refreshRate;
}

int     VideoMode::getWidth() const
{
    return (this->_width);
}

int     VideoMode::getHeight() const
{
    return (this->_height);
}

int     VideoMode::getRedBits() const
{
    return (this->_redBits);
}

int     VideoMode::getGreenBits() const
{
    return (this->_greenBits);
}

int     VideoMode::getBlueBits() const
{
    return (this->_blueBits);
}

int     VideoMode::getRefreshRate() const
{
    return (this->_refreshRate);
}

void    VideoMode::setWidth(int width)
{
    this->_width = width;
}

void    VideoMode::setHeight(int height)
{
    this->_height = height;
}

void    VideoMode::setRedBits(int redBits)
{
    this->_redBits = redBits;
}

void    VideoMode::setGreenBits(int greenBits)
{
    this->_greenBits = greenBits;
}

void    VideoMode::setBlueBits(int blueBits)
{
    this->_blueBits = blueBits;
}

void    VideoMode::setRefreshRate(int refreshRate)
{
    this->_refreshRate = refreshRate;
}

std::string     VideoMode::toString(const VideoMode& object)
{
    std::ostringstream   stream;

    stream << object;
    return (stream.str());
}

const char*     VideoMode::toConstString(const VideoMode& object)
{
    std::string string = VideoMode::toString(object);
    const char* charArray = _strdup(string.c_str());

    return (charArray);
}

std::ostream&   operator<<(std::ostream& os, const VideoMode& object)
{
    os << std::to_string(object.getWidth()) << 'x' << std::to_string(object.getHeight());
    os << ' ' << std::to_string(object.getRedBits()) << '/' << std::to_string(object.getGreenBits()) << '/' << std::to_string(object.getBlueBits());
    os << ' ' << '(' << std::to_string(object.getRefreshRate()) << "Hz" << ')';

    return (os);
}

bool            operator==(const VideoMode& first, const VideoMode& second)
{
    bool    result = true;

    result &= (first.getWidth() == second.getWidth());
    result &= (first.getHeight() == second.getHeight());
    result &= (first.getRedBits() == second.getRedBits());
    result &= (first.getGreenBits() == second.getGreenBits());
    result &= (first.getBlueBits() == second.getBlueBits());
    result &= (first.getRefreshRate() == second.getRefreshRate());

    return (result);
}