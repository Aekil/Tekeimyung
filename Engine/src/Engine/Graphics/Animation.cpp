#include <Engine/Graphics/Animation.hpp>

Animation::Animation(): _loop(true) {}

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
