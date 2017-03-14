/*
** @Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Core/ScriptFactory.hpp>

class GameManager;

class Spawner final : public BaseScript
{
public:
    Spawner() = default;
    ~Spawner() = default;

    void start() override final;
    void update(float dt) override final;

private:
    std::vector<glm::vec3> getClosestPath() const;

    GameManager* _gameManager{nullptr};
    sTransformComponent* _transform;

    std::vector<glm::vec3> _closestPath;
};

REGISTER_SCRIPT(Spawner);
