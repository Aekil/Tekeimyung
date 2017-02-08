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

    virtual void OnCollisionEnter(Entity* entity) {};
    virtual void OnCollisionExit(Entity* entity) {};

    virtual void OnHoverEnter() {};
    virtual void OnHoverExit() {};

    void SetEntity(Entity* entity);

public:
    bool isInitialized;
protected:
    Entity* entity;
    Keyboard& keyboard = GameWindow::getInstance()->getKeyboard();
    Mouse& mouse = GameWindow::getInstance()->getMouse();

    template<typename componentType>
    componentType* getComponent() const
    {
        return this->entity->getComponent<componentType>();
    }

    virtual Entity* Instantiate(std::string, glm::vec3 pos = glm::vec3(0,0,0));

    virtual void Destroy();
    virtual void Destroy(Entity*);
};

