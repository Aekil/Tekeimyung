/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

class CameraScript : public BaseScript
{
public:
    CameraScript() = default;
    ~CameraScript() = default;

    void start() override final;
    void update(float dt) override final;

private:
    sCameraComponent*   _cameraComp;
    float               _scrollSpeed;
    double              _lastScrollOffset;
    bool                _followEntity = false;
    Entity*             _entityToFollow = nullptr;
    glm::vec3           _initialPosition;
};

REGISTER_SCRIPT(CameraScript);
