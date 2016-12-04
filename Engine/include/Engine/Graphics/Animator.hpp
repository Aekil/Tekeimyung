#pragma once

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

    AnimationPtr                        getAnimation(const std::string& name) const;
    const std::vector<AnimationPtr>&    getAnimations() const;
    std::vector<AnimationPtr>&          getAnimations();

    bool                                play(const std::string& name);
    bool                                isPlaying(const std::string& name) const;
    bool                                isPlaying() const;

    AnimationPtr                        getCurrentAnimation() const;

    void                                stop();
    void                                reset();
    void                                update(float elapsedTime);

private:
    std::vector<AnimationPtr>           _animations;

    AnimationPtr                        _currentAnimation;
};
