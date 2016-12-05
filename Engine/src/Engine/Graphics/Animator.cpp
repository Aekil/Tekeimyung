#include <Engine/Utils/Logger.hpp>

#include <Engine/Graphics/Animator.hpp>

Animator::Animator(): _currentAnimation(nullptr) {}

Animator::Animator(const Animator& rhs): _currentAnimation(nullptr)
{
    _animations.clear();
    _currentAnimation = nullptr;

    uint32_t nbAnimations = (uint32_t)rhs.getAnimations().size();
    for (uint32_t i = 0; i < nbAnimations; ++i)
    {
        AnimationPtr animation = rhs.getAnimations()[i];
        addAnimation(std::make_shared<Animation>(*animation));
    }
}

Animator::~Animator() {}

Animator&   Animator::operator=(const Animator& rhs)
{
    _animations.clear();
    _currentAnimation = nullptr;

    uint32_t nbAnimations = (uint32_t)rhs.getAnimations().size();
    for (uint32_t i = 0; i < nbAnimations; ++i)
    {
        AnimationPtr animation = rhs.getAnimations()[i];
        addAnimation(std::make_shared<Animation>(*animation));
    }

    return (*this);
}

void    Animator::addAnimation(AnimationPtr animation)
{
    _animations.push_back(animation);
}

void    Animator::removeAnimation(AnimationPtr animation)
{
    if (animation == _currentAnimation)
    {
        stop();
        update(0);
        _currentAnimation = nullptr;
    }

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

uint32_t    Animator::getAnimationsNb() const
{
    return ((uint32_t)_animations.size());
}

bool    Animator::play(const std::string& name, bool loop)
{
    if (_currentAnimation &&
        _currentAnimation->getName() == name && // The animation is already playing
        _currentAnimation->isPlaying() &&
        _currentAnimation->isLoop() == loop) // The animation loop did not change
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
    {
        _currentAnimation->reset();
        _currentAnimation->update(0);
    }

    // Set new played animation
    _currentAnimation = animation;
    _currentAnimation->reset();
    _currentAnimation->isLoop(loop);

    return (true);
}

bool    Animator::isPlaying(const std::string& name) const
{
    return (_currentAnimation &&
        _currentAnimation->getName() == name &&
        _currentAnimation->isPlaying());
}

bool    Animator::isPlaying() const
{
    return (_currentAnimation != nullptr &&
        _currentAnimation->isPlaying());
}

AnimationPtr    Animator::getCurrentAnimation() const
{
    return (_currentAnimation);
}

void    Animator::reset()
{
    if (_currentAnimation)
        _currentAnimation->reset();
}

void    Animator::stop()
{
    reset();
    update(0);
    _currentAnimation = nullptr;
}


void    Animator::update(float elapsedTime)
{
    if (_currentAnimation)
        _currentAnimation->update(elapsedTime);
}
