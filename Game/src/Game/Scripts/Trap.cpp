/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/Trap.hpp>

void Trap::onCollisionEnter(Entity* entity)
{
    if (entity->getTag() == "Enemy")
    {
        sScriptComponent* script = entity->getComponent<sScriptComponent>();
        Enemy* enemy = script ? script->getScript<Enemy>("Enemy") : nullptr;

        enemy->takeDamage(this->damage);
        this->usage--;

        if (this->usage <= 0)
        {
            this->Destroy();
        }
    }
}
