/**
* @Author   Guillaume Labey
*/

#include <Engine/Graphics/Animation.hpp>

Animation::Animation(const std::string& name): _name(name), _loop(true), _isPlaying(false) {}

Animation::Animation(const Animation& rhs)
{
    _name = rhs.getName();
    _loop = rhs.isLoop();
    _isPlaying = rhs.isPlaying();

    for (const auto& paramAnimation: rhs.getParamsAnimations())
    {
        _paramsAnimations.push_back(paramAnimation->clone());
    }
}

Animation::~Animation() {}

void    Animation::addParamAnimation(IParamAnimationPtr paramAnimation)
{
    paramAnimation->reset();
    _paramsAnimations.push_back(paramAnimation);
}

void    Animation::removeParamAnimation(IParamAnimationPtr paramAnimation)
{
    reset();
    update(0);

    _paramsAnimations.erase(std::find(_paramsAnimations.begin(), _paramsAnimations.end(), paramAnimation));
    if (_paramsAnimations.size() == 0)
        _isPlaying = false;
}

void    Animation::removeParamAnimation(const std::string& name)
{
    IParamAnimationPtr paramAnimation = getParamAnimation(name);
    removeParamAnimation(paramAnimation);
}

IParamAnimationPtr  Animation::getParamAnimation(const std::string& name)
{
    for (const IParamAnimationPtr paramAnimation: _paramsAnimations)
    {
        if (paramAnimation->getName() == name)
            return (paramAnimation);
    }

    return (nullptr);
}

void    Animation::update(float elapsedTime)
{
    _isPlaying = false;
    for (auto paramAnimation: _paramsAnimations)
    {
        // ParamAnimation::update return true if the param animation is not finished
        if (paramAnimation->update(elapsedTime))
            _isPlaying = true;
    }

    if (!_isPlaying && _loop)
        reset();
}

void    Animation::reset()
{
    _isPlaying = true;
    for (auto paramAnimation: _paramsAnimations)
    {
        paramAnimation->reset();
    }
}

const std::string&  Animation::getName() const
{
    return (_name);
}

void    Animation::setName(const std::string& name)
{
    _name = name;
}

bool    Animation::isLoop() const
{
    return (_loop);
}

void    Animation::isLoop(bool isLoop)
{
    _loop = isLoop;
}

bool    Animation::isPlaying() const
{
    return (_isPlaying);
}

std::vector<IParamAnimationPtr >& Animation::getParamsAnimations()
{
    return (_paramsAnimations);
}

const std::vector<IParamAnimationPtr >& Animation::getParamsAnimations() const
{
    return (_paramsAnimations);
}
