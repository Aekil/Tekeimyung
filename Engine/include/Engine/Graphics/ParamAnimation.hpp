#include <vector>
#include <memory>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Helper.hpp>

struct sKeyFrame;

class IParamAnimation
{
public:
    IParamAnimation(const std::string& name): _name(name) {}
    virtual ~IParamAnimation() {}

    virtual bool            update() { return (false); }
    virtual void            reset() {}
    const std::string&      getName() { return (_name); }

private:
    std::string             _name;
};

// Param animation
// Ex: position animation
// It contains all the key frames to animate the param
template <typename T>
class ParamAnimation: public IParamAnimation
{
public:
    // Easing equation interpolations between ParamAnimation::startValue and sKeyFrame::value
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
        T value;
        eEasing easing;
    };

public:
    ParamAnimation(const std::string& name, T* param);
    virtual ~ParamAnimation();

    void                    addKeyFrame(sKeyFrame& keyFrame);
    std::vector<sKeyFrame>& getKeyFrames() { return (_keyFrames); }
    const std::vector<sKeyFrame>& getKeyFrames() const { return (_keyFrames); }

    void                    setParam(T* param) { _param = param; reset(); }

    virtual bool            update();
    virtual void            reset();

private:
    T                       getNewValue(const sKeyFrame& keyFrame);

private:
    std::vector<sKeyFrame>  _keyFrames;
    Timer                   _timer;

    // Param updated during the animation
    T*                      _param;

    // Last value from getNewValue()
    T                       _lastValue;

    // Start value used for interpolation with  sKeyFrame::value
    T                       _startValue;

    uint16_t                _currentKeyFrame;
};

#include <Engine/Graphics/ParamAnimation.inl>
