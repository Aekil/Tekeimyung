#include <Engine/Utils/Logger.hpp>

#include <Engine/Graphics/Animator.hpp>

Animator::Animator(): _currentAnimation(nullptr) {}

Animator::Animator(const Animator& rhs): _currentAnimation(nullptr)
{
    _animations.clear();

    uint32_t nbAnimations = (uint32_t)rhs.getAnimations().size();
    for (uint32_t i = 0; i < nbAnimations; ++i)
    {
        AnimationPtr animation = rhs.getAnimations()[i];
        addAnimation(std::make_shared<Animation>(*animation));
    }

    if (nbAnimations > 0)
        _currentAnimation = _animations[0];
}

Animator::~Animator() {}

Animator&   Animator::operator=(const Animator& rhs)
{
    _animations.clear();

    uint32_t nbAnimations = (uint32_t)rhs.getAnimations().size();
    for (uint32_t i = 0; i < nbAnimations; ++i)
    {
        AnimationPtr animation = rhs.getAnimations()[i];
        addAnimation(std::make_shared<Animation>(*animation));
    }

    if (nbAnimations > 0)
        _currentAnimation = _animations[0];

    return (*this);
}

void    Animator::addAnimation(AnimationPtr animation)
{
    _animations.push_back(animation);
}

void    Animator::removeAnimation(AnimationPtr animation)
{
    if (animation == _currentAnimation)
        _currentAnimation = nullptr;

    _animations.erase(std::find(_animations.begin(), _animations.end(), animation));
}

void    Animator::removeAnimation(const std::string& name)
{
    AnimationPtr animation = getAnimation(name);
    removeAnimation(animation);
}

AnimationPtr    Animator::getAnimation(const std::string& name) const
{
    for (const AnimationPtr animation: _animations)
    {
        if (animation->getName() == name)
            return (animation);
    }

    return (nullptr);
}

const std::vector<AnimationPtr>&    Animator::getAnimations() const
{
    return (_animations);
}

std::vector<AnimationPtr>&  Animator::getAnimations()
{
    return (_animations);
}

bool    Animator::play(const std::string& name)
{
    // The animation is already playing
    if (_currentAnimation && _currentAnimation->getName() == name)
        return (true);

    // Get new animation pointer
    AnimationPtr animation = getAnimation(name);

    // The animation does not exist
    if (!animation)
    {
        LOG_INFO("Can't play animation \"%s\": this animation doesn not exist", name.c_str());
        return (false);
    }

    // An animation is already playing, reset it to reset transforms
    if (_currentAnimation)
        _currentAnimation->reset();

    // Set new played animation
    _currentAnimation = animation;

    return (true);
}

bool    Animator::isPlaying(const std::string& name) const
{
    return (_currentAnimation && _currentAnimation->getName() == name);
}

bool    Animator::isPlaying() const
{
    return (_currentAnimation != nullptr);
}

AnimationPtr    Animator::getCurrentAnimation() const
{
    return (_currentAnimation);
}

void    Animator::stop()
{
    if (_currentAnimation)
        _currentAnimation->reset();
}

void    Animator::update()
{
    if (_currentAnimation)
        _currentAnimation->update();
}
