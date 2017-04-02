/**
* @Author   Mathieu Chassara
*/

#include    <ostream>
#include    <vector>

#include    <GLFW/glfw3.h>

class   VideoMode
{
public:
    VideoMode();
    ~VideoMode();

    VideoMode(const GLFWvidmode* vidmode);

    int     getWidth() const;
    int     getHeight() const;
    int     getRedBits() const;
    int     getGreenBits() const;
    int     getBlueBits() const;
    int     getRefreshRate() const;

    void    setWidth(int width);
    void    setHeight(int height);
    void    setRedBits(int redBits);
    void    setGreenBits(int greenBits);
    void    setBlueBits(int blueBits);
    void    setRefreshRate(int refreshRate);

    static  std::string toString(const VideoMode& object);
    static const char*  toConstString(const VideoMode& object);
private:
    int _width;
    int _height;
    int _redBits;
    int _greenBits;
    int _blueBits;
    int _refreshRate;
};

std::ostream&   operator<<(std::ostream& os, const VideoMode& object);
bool            operator==(const VideoMode& first, const VideoMode& second);