/**
* @Author   Guillaume Labey
*/

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Engine/Utils/Timer.hpp>
#include <Engine/Utils/Helper.hpp>

// Generate a list
// string: "PLAYER", "TILE1", "TILE2"
// enum: PLAYER, TILE1, TILE2
#define EASING_TYPES(PROCESS)\
    PROCESS(NONE)\
    PROCESS(EASE_IN)\
    PROCESS(EASE_OUT)\
    PROCESS(EASE_IN_OUT)\
    PROCESS(EXPONENTIAL)\

struct sKeyFrame;

class IParamAnimation
{
public:
    enum class eInterpolationType: char
    {
        RELATIVE = 0,
        ABSOLUTE = 1
    };


    REGISTER_ENUM(eEasing, char, EASING_TYPES);

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

REGISTER_ENUM_MANAGER(IParamAnimation::eEasing, EASING_TYPES);

// Param animation
// Ex: position animation
// It contains all the key frames to animate the param
template <typename T>
class ParamAnimation: public IParamAnimation
{
public:
    // Key frame
    struct sKeyFrame
    {
        uint32_t id;

        float time;
        T value;
        eEasing easing;
    };

public:
    ParamAnimation(const std::string& name, T* param, eInterpolationType type = eInterpolationType::RELATIVE);
    virtual ~ParamAnimation();

    static bool             compareKeyFramesByTime(const sKeyFrame& a, const sKeyFrame& b);

    void                    addKeyFrame(const sKeyFrame& keyFrame, bool sort = true);
    void                    sortKeyFrames();

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
    float                   _lastKeyTime;

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

    uint32_t                _lastFrameId;
};

#include <Engine/Graphics/ParamAnimation.inl>
