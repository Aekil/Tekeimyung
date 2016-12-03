#include <Engine/Graphics/Animation.hpp>

Animation::Animation(const std::string& name): _name(name), _loop(true) {}

Animation::Animation(const Animation& rhs)
{
    _name = rhs.getName();
    _loop = rhs.isLoop();

    for (const auto& paramAnimation: rhs.getParamsAnimations())
    {
        _paramsAnimations.push_back(paramAnimation->clone());
    }
}

Animation::~Animation() {}

void    Animation::addParamAnimation(std::shared_ptr<IParamAnimation> paramAnimation)
{
    paramAnimation->reset();
    _paramsAnimations.push_back(paramAnimation);
}

void    Animation::update()
{
    bool resetAnimation = true;
    for (auto paramAnimation: _paramsAnimations)
    {
        // ParamAnimation::update return true if the param animation is not finished
        if (paramAnimation->update())
            resetAnimation = false;
    }

    if (resetAnimation && _loop)
        reset();
}

void    Animation::reset()
{
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

std::vector<std::shared_ptr<IParamAnimation> >& Animation::getParamsAnimations()
{
    return (_paramsAnimations);
}

const std::vector<std::shared_ptr<IParamAnimation> >& Animation::getParamsAnimations() const
{
    return (_paramsAnimations);
}
