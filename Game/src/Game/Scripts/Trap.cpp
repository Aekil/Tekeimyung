/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Trap.hpp>

void Trap::OnCollisionEnter(Entity* entity)
{
    if (entity->getComponent<sNameComponent>()->value == "PLAYER")
    {
        auto scriptComponent = entity->getComponent<sScriptComponent>();

        if (std::find(scriptComponent->scriptNames.cbegin(), scriptComponent->scriptNames.cend(), "Player") != scriptComponent->scriptNames.cend())
        {
            auto index = std::find(scriptComponent->scriptNames.cbegin(), scriptComponent->scriptNames.cend(), "Player") - scriptComponent->scriptNames.begin();

            Player* player = (Player*)scriptComponent->scriptInstances[index];
            player->TakeDamage(this->damage);
        }
    }
}