/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/ParamAnimation.hpp>

using IParamAnimationPtr = std::shared_ptr<IParamAnimation>;

class Animation
{
public:
    Animation(const std::string& name);
    Animation(const Animation& rhs);
    ~Animation();

    void                    addParamAnimation(IParamAnimationPtr paramAnimation);

    void                    removeParamAnimation(IParamAnimationPtr paramAnimation);
    void                    removeParamAnimation(const std::string& name);

    IParamAnimationPtr      getParamAnimation(const std::string& name);

    void                    update(float elapsedTime);
    void                    reset();

    const std::string&      getName() const;
    void                    setName(const std::string& name);

    bool                    isLoop() const;
    void                    isLoop(bool isLoop);

    bool                    isPlaying() const;
    void                    isPlaying(bool isPlaying);

    std::vector<IParamAnimationPtr >&  getParamsAnimations();
    const std::vector<IParamAnimationPtr >&  getParamsAnimations() const;

private:
    // An animation has got multiple params animations
    // Ex: position, rotation and scale animation
    std::vector<IParamAnimationPtr >  _paramsAnimations;

    // Animation name
    std::string                     _name;
    bool                            _loop;

    bool                            _isPlaying;
};
