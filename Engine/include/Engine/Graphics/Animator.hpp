/**
* @Author   Guillaume Labey
*/

#pragma once

#include <unordered_map>
#include <Engine/Graphics/Animation.hpp>

using AnimationPtr = std::shared_ptr<Animation>;

class Animator
{
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

    bool                                play(const std::string& name, bool loop = true);
    bool                                stop(const std::string& name);
    bool                                isPlaying(const std::string& name) const;
    bool                                isPlaying(AnimationPtr animation) const;
    bool                                isPlaying() const;

    void                                reset();
    void                                update(float elapsedTime);

private:
    AnimationPtr                        getAnimation(const std::string& name) const;

    std::vector<AnimationPtr>*          getPlayedLayer(const std::string& layer);
    void                                removePlayedLayer(const std::string& layer);

private:
    std::vector<AnimationPtr>           _animations;

    // Store played animations by layers names
    std::unordered_map<std::string, std::vector<AnimationPtr>> _playedLayers;
};
