/**
* @Author   Guillaume Labey
*/

#pragma once

#include <Engine/Graphics/ParamAnimation.hpp>

using IParamAnimationPtr = std::shared_ptr<IParamAnimation>;

class Animation
{
public:
    Animation(const std::string& name = "animation", const std::string& layer = "DEFAULT");
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

    const std::string&      getLayer() const;
    void                    setLayer(const std::string& layer);

    bool                    isLoop() const;
    void                    isLoop(bool isLoop);

    bool                    isPlaying() const;

    std::vector<IParamAnimationPtr >&  getParamsAnimations();
    const std::vector<IParamAnimationPtr >&  getParamsAnimations() const;

private:
    void                    isPlaying(bool isPlaying);

private:
    // An animation has got multiple params animations
    // Ex: position, rotation and scale animation
    std::vector<IParamAnimationPtr >  _paramsAnimations;

    // Animation name
    std::string                     _name;
    std::string                     _layer;
    bool                            _loop;

    bool                            _isPlaying;
};
