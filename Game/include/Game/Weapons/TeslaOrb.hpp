/**
* @Author   Mathieu Chassara
*/

#include    <Engine/Core/ScriptFactory.hpp>

#include    <Game/Attibutes/Attribute.hpp>

struct sRigidBodyComponent;

class       TeslaOrb final : public BaseScript
{
public:
    TeslaOrb() = default;
    ~TeslaOrb() override final = default;

    void    start() override final;
    void    update(float deltaTime) override final;
    void    onCollisionEnter(Entity* entity) override final;

    //  Orb functions
    void    setPosition(const glm::vec3& direction);
    void    setDirection(const glm::vec3& direction);

    bool    hasReachedItsRange();
    void    triggerNanoboost();
    void    triggerExplosion();

private:
    sRigidBodyComponent*    _rigidBody;
    Attribute*              _speed;
    Attribute*              _range;

    glm::vec3               _startPosition;
};

REGISTER_SCRIPT(TeslaOrb);
