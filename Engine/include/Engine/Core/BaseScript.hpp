/**
* @Author   Simon AMBROISE
*/

#pragma once

#include <glm/vec3.hpp>

#include <ECS/Entity.hpp>

#include <Engine/Utils/Logger.hpp>

#include <Engine/Window/GameWindow.hpp>

#define KB_P(x)         this->keyboard.isPressed(x) 

class BaseScript
{
public:
    BaseScript();
    virtual ~BaseScript() {};
    virtual void Start() = 0;
    virtual void Update(float dt) = 0;

    virtual void OnCollisionEnter(int entityId) {};
    virtual void OnCollisionExit(int entityId) {};

    void SetEntity(Entity* entity);

protected:
    Entity* entity;
    Keyboard& keyboard = GameWindow::getInstance()->getKeyboard();

    template<typename componentType>
    componentType* getComponent() const
    {
        return this->entity->getComponent<componentType>();
    }

    virtual void Instantiate(std::string, glm::vec3 pos);

    virtual void Destroy();

};

