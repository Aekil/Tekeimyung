/**
* @Author   Guillaume Labey
*/

#include <algorithm>
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

bool    Animator::play(const std::string& name, bool loop, bool stopSameLayer)
{
    AnimationPtr animation = getAnimation(name);

    if (!animation)
    {
        LOG_WARN("Can't play animation \"%s\": this animation doesn not exist", name.c_str());
        return (false);
    }

    // Stop all animations on the same layer
    if (stopSameLayer)
    {
        sAnimationLayer* playedLayer = getPlayedLayer(animation->getLayer());
        if (playedLayer)
        {
            for (auto& layerAnimation: playedLayer->animations)
            {
                layerAnimation->reset();
                layerAnimation->update(0);
            }

            playedLayer->animations.clear();
        }
    }
    else if (isPlaying(animation))
    {
        removePlayedAnimation(animation);
    }

    // Set new played animation
    _playedLayers[animation->getLayer()].animations.push_back(animation);
    animation->reset();
    animation->isLoop(loop);

    return (true);
}

bool    Animator::playQueued(const std::string& name, bool loop)
{
    return (play(name, loop, false));
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
        removePlayedAnimation(animation);
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
    for (const auto& playedLayer: _playedLayers)
    {
        auto& layerAnimations = playedLayer.second.animations;
        if (playedLayer.first == animation->getLayer() &&
            std::find(layerAnimations.cbegin(), layerAnimations.cend(), animation) != layerAnimations.cend())
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
    for (const auto& playedLayer: _playedLayers)
    {
        for (auto& animation: playedLayer.second.animations)
        {
            animation->reset();
            animation->update(0);
        }
    }
    _playedLayers.clear();
}

void    Animator::update(float elapsedTime)
{
    auto& playedLayer = _playedLayers.begin();
    for (playedLayer; playedLayer != _playedLayers.end();)
    {
        auto& layer = playedLayer->second;
        AnimationPtr animation = layer.animations[layer.currentAnimation];

        // The animation is finished
        if (animation->update(elapsedTime))
        {
            // Remove the animation from the played layer if there is no loop
            if (!animation->isLoop())
            {
                layer.animations.erase(layer.animations.begin() + layer.currentAnimation);
                layer.currentAnimation = std::min(layer.currentAnimation, (uint32_t)layer.animations.size() - 1);
            }
            else
            {
                layer.currentAnimation++;
                if (layer.currentAnimation >= layer.animations.size())
                {
                    layer.currentAnimation = 0;
                }
            }
        }

        // Remove layer from played layers
        if (layer.animations.size() == 0)
        {
            _playedLayers.erase(playedLayer++);
        }
        else
        {
            ++playedLayer;
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

Animator::sAnimationLayer*  Animator::getPlayedLayer(const std::string& layer)
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

void    Animator::removePlayedAnimation(AnimationPtr animation)
{
    sAnimationLayer* playedLayer = getPlayedLayer(animation->getLayer());
    ASSERT(playedLayer != nullptr, "A played animation should be on a layer");

    auto& layerAnimation = std::find(playedLayer->animations.cbegin(), playedLayer->animations.cend(), animation);
    ASSERT(layerAnimation != playedLayer->animations.end(), "The animation should be on the layer list");

    animation->reset();
    animation->update(0);

    playedLayer->animations.erase(layerAnimation);
    if (playedLayer->animations.size() == 0)
    {
        removePlayedLayer(animation->getLayer());
    }
}
