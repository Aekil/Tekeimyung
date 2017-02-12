/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/Animation.hpp>

using AnimationPtr = std::shared_ptr<Animation>;

class Animator
{
private:
    struct sAnimationLayer
    {
        std::vector<AnimationPtr> animations;
        uint32_t currentAnimation = 0;
    };

public:
    Animator();
    Animator(const Animator& rhs);
    ~Animator();

    Animator&                           operator=(const Animator& rhs);

    void                                addAnimation(AnimationPtr animation);

    void                                removeAnimation(AnimationPtr animation);
    void                                removeAnimation(const std::string& name);

    const std::vector<AnimationPtr>&    getAnimations() const;
    uint32_t                            getAnimationsNb() const;

    bool                                play(const std::string& name, bool loop = true, bool stopSameLayer = true);
    bool                                playQueued(const std::string& name, bool loop = true);
    bool                                stop(const std::string& name);
    bool                                isPlaying(const std::string& name) const;
    bool                                isPlaying(AnimationPtr animation) const;
    bool                                isPlaying() const;

    void                                reset();
    void                                update(float elapsedTime);

private:
    AnimationPtr                        getAnimation(const std::string& name) const;

    sAnimationLayer*                    getPlayedLayer(const std::string& layer);
    void                                removePlayedLayer(const std::string& layer);
    void                                removePlayedAnimation(AnimationPtr animation);

private:
    std::vector<AnimationPtr>           _animations;

    // Store played animations by layers names
    std::unordered_map<std::string, sAnimationLayer> _playedLayers;
};
