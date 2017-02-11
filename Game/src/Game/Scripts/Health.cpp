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

bool Health::TakeDamage(int health)
{
    this->health -= health;

    if (this->health <= 0)
    {
        this->Death();
        return (true);
    }

    return (false);
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
    maxScale = _healthBarEmptyTransform->scale.x;
}

void Health::update(sTransformComponent* transform)
{
    Camera* camera = Renderer::getInstance()->getCurrentCamera();
    auto& gameWindow = GameWindow::getInstance();

    if (!camera)
        return;

    auto& screen = camera->getScreen();
    glm::vec4 viewport(gameWindow->getViewport());
    glm::mat4 model = glm::translate(glm::mat4(1.0), transform->pos);
    glm::vec3 screenPos = glm::project(_objCenter * transform->scale, camera->getView() * model, camera->getProj(), viewport);

    float healthSize = _healthRender->getModel()->getMax().x - _healthRender->getModel()->getMin().x;
    screenPos.x -= healthSize * _healthBarEmptyTransform->scale.x / 2.0f;
    screenPos.y += 4.0f;

    this->_healthBarTransform->pos.x = screenPos.x;
    this->_healthBarTransform->pos.y = screenPos.y;
    this->_healthBarTransform->scale.x = (float)health / (float)maxHealth * maxScale;
    this->_healthBarTransform->needUpdate = true;
    this->_healthBarEmptyTransform->pos.x = screenPos.x;
    this->_healthBarEmptyTransform->pos.y = screenPos.y;
    this->_healthBarEmptyTransform->needUpdate = true;
}
