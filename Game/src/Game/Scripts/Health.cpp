/*
** @Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Game/Scripts/Health.hpp>

Health::Health()
{
    _healthBarEmpty = nullptr;
    _healthBar = nullptr;
}

Health::~Health()
{
    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(_healthBarEmpty);
    em->destroyEntityRegister(_healthBar);
}

bool Health::takeDamage(int damage)
{
    this->health -= damage;

    if (this->health <= 0)
    {
        this->death();
        return (true);
    }

    return (false);
}

int Health::getHealth() const
{
    return (health);
}

void Health::init(sTransformComponent* transform, sRenderComponent* render)
{
    this->_healthBarEmpty = EntityFactory::createEntity("HEALTH_EMPTY");
    this->_healthBar = EntityFactory::createEntity("HEALTH");
    this->_healthBarTransform = _healthBar->getComponent<sTransformComponent>();
    this->_healthBarEmptyTransform = _healthBarEmpty->getComponent<sTransformComponent>();
    this->_healthRender = _healthBar->getComponent<sRenderComponent>();

    _objCenter.x = render->getModel()->getMin().x + ((render->getModel()->getMax().x - render->getModel()->getMin().x) / 2.0f);
    _objCenter.y = render->getModel()->getMax().y;
    _objCenter.z = render->getModel()->getMin().z + ((render->getModel()->getMax().z - render->getModel()->getMin().z) / 2.0f);
    maxScale = _healthBarEmptyTransform->getScale().x;
}

void Health::update(sTransformComponent* transform)
{
    Camera* camera = Renderer::getInstance()->getCurrentCamera();
    auto& gameWindow = GameWindow::getInstance();

    if (!camera)
        return;

    glm::vec4 viewport = glm::vec4(camera->getViewport().offset.x,
                                camera->getViewport().offset.y,
                                camera->getViewport().extent.width,
                                camera->getViewport().extent.height);
    glm::mat4 model = glm::translate(glm::mat4(1.0), transform->getPos());
    glm::vec3 screenPos = glm::project(_objCenter * transform->getScale(), camera->getView() * model, camera->getProj(), viewport);

    float healthSize = _healthRender->getModel()->getMax().x - _healthRender->getModel()->getMin().x;
    float healthEmptySize = healthSize * _healthBarEmptyTransform->getScale().x / 2.0f;
    screenPos.y += 4.0f;

    this->_healthBarTransform->setPos(glm::vec3(screenPos.x, screenPos.y, this->_healthBarTransform->getPos().z));
    this->_healthBarTransform->setScale(glm::vec3((float)health / (float)maxHealth * maxScale,
                                                    this->_healthBarTransform->getScale().y,
                                                    this->_healthBarTransform->getScale().z));
    this->_healthBarTransform->translate(-glm::vec3(healthEmptySize - ((this->_healthBarTransform->getScale().x * healthSize) / 2.0f),
                                                    0.0f, 0.0f));
    this->_healthBarEmptyTransform->setPos(glm::vec3(screenPos.x, screenPos.y, this->_healthBarEmptyTransform->getPos().z));
}
