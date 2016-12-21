/**
* @Author   Simon AMBROISE
*/

#include <Engine/Core/BasicClass.hpp>

BasicClass::BasicClass(Entity* entity) : entity(entity)
{
    this->Start();
}

BasicClass::~BasicClass()
{

}