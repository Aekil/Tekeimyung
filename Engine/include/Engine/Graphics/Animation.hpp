#pragma once

#include <Engine/Graphics/ParamAnimation.hpp>

class Animation
{
public:
    Animation(const std::string& name);
    Animation(const Animation& rhs);
    ~Animation();

    void                    addParamAnimation(std::shared_ptr<IParamAnimation> paramAnimation);

    void                    update();
    void                    reset();

    const std::string&      getName() const;
    void                    setName(const std::string& name);

    bool                    isLoop() const;

    std::vector<std::shared_ptr<IParamAnimation> >&  getParamsAnimations();
    const std::vector<std::shared_ptr<IParamAnimation> >&  getParamsAnimations() const;

private:
    // An animation has got multiple params animations
    // Ex: position, rotation and scale animation
    std::vector<std::shared_ptr<IParamAnimation> >  _paramsAnimations;

    // Animation name
    std::string                     _name;
    bool                            _loop;
};
