#include <Engine/Components.hh>

#include <Game/Scripts/Castle.hpp>

void Castle::Start()
{
    
}

void Castle::Update(float dt)
{
    if (this->health <= 0)
        this->Destroy();
}

void Castle::OnCollisionEnter(Entity* entity)
{
    if (entity->getComponent<sNameComponent>()->value == "PLAYER")
    {
        LOG_DEBUG("Damage done");
        this->health -= 25;
    }
}