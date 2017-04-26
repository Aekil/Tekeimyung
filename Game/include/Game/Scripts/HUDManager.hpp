/*
** @Author : Mathieu Chassara
*/

#pragma once

#include    <Engine/Core/BaseScript.hpp>

class       HUDManager final : public BaseScript
{
public:
    HUDManager() = default;
    virtual ~HUDManager() = default;

    //      BaseScript functions
public:
    void    start() override final;
    void    update(float deltaTime) override final;

private:

    std::vector<Entity*>    _displayableEntities;
};