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

    virtual bool            update(float elapsedTime) = 0;
    virtual void            reset() = 0;
    virtual std::shared_ptr<IParamAnimation> clone() = 0;
    const std::string&      getName() { return (_name); }

protected:
    std::string             _name;
};

// Param animation
// Ex: position animation
// It contains all the key frames to animate the param
template <typename T>
class ParamAnimation: public IParamAnimation
{
public:
    enum class eInterpolationType: char
    {
        RELATIVE = 0,
        ABSOLUTE = 1
    };

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
    ParamAnimation(const std::string& name, T* param, eInterpolationType type = eInterpolationType::RELATIVE);
    virtual ~ParamAnimation();

    void                    addKeyFrame(const sKeyFrame& keyFrame);

    void                    removeKeyFrame(std::size_t index)
    {
        // Reverse param animation before removing the last key frame
        if (_keyFrames.size() == 1)
        {
            reset();
            update(0);
        }

        _keyFrames.erase(_keyFrames.begin() + index);
    }

    std::vector<sKeyFrame>& getKeyFrames()
    {
        return (_keyFrames);
    }
    const std::vector<sKeyFrame>& getKeyFrames() const
    {
        return (_keyFrames);
    }

    const sKeyFrame& getKeyFrame(std::size_t index) const
    {
        return _keyFrames[index];
    }
    sKeyFrame&      getKeyFrame(std::size_t index)
    {
        return _keyFrames[index];
    }

    const sKeyFrame* getCurrentKeyFrame() const
    {
        if (_currentKeyFrame > _keyFrames >= _keyFrames.size())
            return (nullptr);
        return (_keyFrames[_currentKeyFrame]);
    }

    void                    setParam(T* param)
    {
        _param = param;

        if (_type == eInterpolationType::ABSOLUTE && _param)
        {
            _initialValue = *_param;
            _startValue = *_param;
            _lastValue = *_param;
        }

        reset();
    }

    virtual bool            update(float elapsedTime);
    virtual void            reset();
    virtual std::shared_ptr<IParamAnimation>    clone();

private:
    T                       getNewValue(const sKeyFrame& keyFrame);

private:
    std::vector<sKeyFrame>  _keyFrames;

    float                   _elapsedTime;

    // Param updated during the animation
    T*                      _param;
    // Initial value of _param
    T                       _initialValue;

    // Last value from getNewValue()
    T                       _lastValue;

    // Start value used for interpolation with sKeyFrame::value
    T                       _startValue;

    uint16_t                _currentKeyFrame;

    // Interpolation type to use
    eInterpolationType      _type;
};

#include <Engine/Graphics/ParamAnimation.inl>
