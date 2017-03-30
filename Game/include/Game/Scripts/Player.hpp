/*
** Author : Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <Engine/Utils/EventSound.hpp>
#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#include <Game/Scripts/Health.hpp>
#include <Game/Attibutes/Attribute.hpp>

class GameManager;
class WaveManager;

class Player final : public BaseScript, public Health
{
public:
    Player() = default;
    ~Player() = default;

private:
    void updateDirection();
    void movement(float elapsedTime);
    void handleShoot(float dt);

    float buildableRadius;
    void levelUp();

public:
    void start() override final;
    void update(float dt) override final;

    virtual void onHoverEnter();
    virtual void onHoverExit();

    void death() override final;

    void onCollisionEnter(Entity* entity) override final;
    void onCollisionExit(Entity* entity) override final;

    void addExperience(int);

private:
    glm::vec3 _direction;
    sTransformComponent* _transform;
    sRenderComponent* _render;
    sRigidBodyComponent* _rigidBody;
    bool _buildEnabled;
    int _damage;
    float _speed;
    tEventSound* _shootSound = nullptr;

    int _experience = 0;
    float _fireRate = 0.5f;
    float _elapsedTime = 0.0f;
    int _nextLevelUp = 100;
    int _level = 1;

    std::map<std::string, Attribute*> _attributes;

    WaveManager* _waveManager{ nullptr };
    GameManager* _gameManager{ nullptr };

    std::map<int, std::pair<std::string, double>> _levelUpReward;
};

REGISTER_SCRIPT(Player);
