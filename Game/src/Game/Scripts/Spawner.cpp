/*
** @Author : Simon AMBROISE
*/

#include <Engine/Components.hh>

#include <Game/Scripts/Spawner.hpp>

void Spawner::start()
{
    auto& pos = this->getComponent<sTransformComponent>()->getPos();

    this->Instantiate("ENEMY", glm::vec3(pos.x, 18.75, pos.z));
}

void Spawner::update(float dt)
{

}
