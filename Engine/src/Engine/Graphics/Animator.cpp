/**
* @Author   Guillaume Labey
*/

#include <Engine/Utils/Logger.hpp>

#include <Engine/Graphics/Animator.hpp>

Animator::Animator() {}

Animator::Animator(const Animator& rhs)
{
    _animations.clear();
    //_currentAnimation = nullptr;

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
    //_currentAnimation = nullptr;

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
    animation->reset();
    animation->update(0);

    _animations.erase(std::find(_animations.begin(), _animations.end(), animation));
}

void    Animator::removeAnimation(const std::string& name)
{
    AnimationPtr animation = getAnimation(name);
    removeAnimation(animation);
}

const std::vector<AnimationPtr>&    Animator::getAnimations() const
{
    return (_animations);
}

uint32_t    Animator::getAnimationsNb() const
{
    return ((uint32_t)_animations.size());
}

bool    Animator::play(const std::string& name, bool loop)
{
    AnimationPtr animation = getAnimation(name);

    if (!animation)
    {
        LOG_WARN("Can't play animation \"%s\": this animation doesn not exist", name.c_str());
        return (false);
    }

    if (isPlaying(animation) &&
        animation->isLoop() == loop) // The animation loop did not change
        return (true);

    // Stop all animations on the same layer
    std::vector<AnimationPtr>* layerAnimations = getPlayedLayer(animation->getLayer());
    if (layerAnimations)
    {
        for (auto& layerAnimation: *layerAnimations)
        {
            layerAnimation->reset();
            layerAnimation->update(0);
        }

        layerAnimations->clear();
    }

    // Set new played animation
    _playedLayers[animation->getLayer()].push_back(animation);
    animation->reset();
    animation->isLoop(loop);

    return (true);
}

bool    Animator::stop(const std::string& name)
{
    AnimationPtr animation = getAnimation(name);

    if (!animation)
    {
        LOG_WARN("Can't play animation \"%s\": this animation doesn not exist", name.c_str());
        return (false);
    }

    if (isPlaying(animation))
    {
        std::vector<AnimationPtr>* layerAnimations = getPlayedLayer(animation->getLayer());
        ASSERT(layerAnimations != nullptr, "A played animation should be on a layer");

        auto& layerAnimation = std::find(layerAnimations->cbegin(), layerAnimations->cend(), animation);
        ASSERT(layerAnimation != layerAnimations->end(), "The animation should be on the layer list");

        animation->reset();
        animation->update(0);

        layerAnimations->erase(layerAnimation);
        if (layerAnimations->size() == 0)
        {
            removePlayedLayer(animation->getLayer());
        }
    }
    return (true);
}

bool    Animator::isPlaying(const std::string& name) const
{
    AnimationPtr animation = getAnimation(name);
    if (!animation)
        return (false);

    return isPlaying(animation);
}

bool    Animator::isPlaying(AnimationPtr animation) const
{
    for (const auto& layerAnimations: _playedLayers)
    {
        if (layerAnimations.first == animation->getLayer() &&
            std::find(layerAnimations.second.cbegin(), layerAnimations.second.cend(), animation) != layerAnimations.second.cend())
        {
            return (true);
        }
    }

    return (false);
}

bool    Animator::isPlaying() const
{
    return (_playedLayers.size() != 0);
}

void    Animator::reset()
{
    for (const auto& layerAnimations: _playedLayers)
    {
        for (auto& animation: layerAnimations.second)
        {
            animation->reset();
            animation->update(0);
        }
    }
    _playedLayers.clear();
}

void    Animator::update(float elapsedTime)
{
    auto& layerAnimations = _playedLayers.begin();
    for (layerAnimations; layerAnimations != _playedLayers.end();)
    {
        auto& animation = layerAnimations->second.begin();
        for (animation; animation != layerAnimations->second.end();)
        {
            (*animation)->update(elapsedTime);
            // Remove animation from played animation
            if (!(*animation)->isPlaying())
                animation = layerAnimations->second.erase(animation);
            else
                ++animation;
        }

        // Remove layer from played layers
        if (layerAnimations->second.size() == 0)
        {
            _playedLayers.erase(layerAnimations++);
        }
        else
        {
            ++layerAnimations;
        }
    }
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

std::vector<AnimationPtr>*  Animator::getPlayedLayer(const std::string& layer)
{
    auto& animationLayer = _playedLayers.find(layer);
    if (animationLayer == _playedLayers.end())
        return (nullptr);

    return &animationLayer->second;
}

void    Animator::removePlayedLayer(const std::string& layer)
{
    _playedLayers.erase(layer);
}
