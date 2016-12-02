#include <vector>
#include <memory>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Helper.hpp>

struct sKeyFrame;

class IParamAnimation
{
public:
    IParamAnimation() {}
    virtual ~IParamAnimation() {}

    virtual bool            update() { return (false); }
    virtual void            reset() {}
};

// Param animation
// Ex: position animation
// It contains all the key frames to animate the param
template <typename T>
class ParamAnimation: public IParamAnimation
{
public:
    // Easing equation interpolations between startValue and endValue
    enum class eEasing: char
    {
        NONE = 0,
        EASE_IN = 1,
        EASE_OUT = 2
    };

    // Key frame
    struct sKeyFrame
    {
        float duration;
        T startValue;
        T endValue;
        eEasing easing;
    };

public:
    ParamAnimation(T* param);
    virtual ~ParamAnimation();

    virtual void            addKeyFrame(sKeyFrame& keyFrame);

    virtual bool            update();
    virtual void            reset();

private:
    T                       getNewValue(const sKeyFrame& keyFrame);

private:
    std::vector<sKeyFrame>  _keyFrames;
    Timer                   _timer;

    // param updated during the animation
    T*                      _param;

    uint16_t                _currentKeyFrame;
};

#include <Engine/Graphics/ParamAnimation.inl>
