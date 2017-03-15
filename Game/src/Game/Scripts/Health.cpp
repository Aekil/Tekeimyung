/*
** @Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Game/Scripts/Health.hpp>

bool Health::takeDamage(int damage)
{
    this->_currentProgress -= damage;

    if (this->_currentProgress <= 0)
    {
        this->death();
        return (true);
    }

    return (false);
}

int Health::getHealth() const
{
    return ((int)_currentProgress);
}

void    Health::setHealth(int health)
{
    _currentProgress = (float)health;
}

void    Health::setMaxHealth(int maxHealth)
{
    _maxProgress = (float)maxHealth;
}

void Health::init(sRenderComponent* render)
{
    ProgressBar::init("HEALTH_EMPTY", "HEALTH");
    _objCenter.x = render->getModel()->getMin().x + ((render->getModel()->getMax().x - render->getModel()->getMin().x) / 2.0f);
    _objCenter.y = render->getModel()->getMax().y;
    _objCenter.z = render->getModel()->getMin().z + ((render->getModel()->getMax().z - render->getModel()->getMin().z) / 2.0f);
    this->_healthRender = _progressBar->getComponent<sRenderComponent>();
}

void Health::update(sTransformComponent* transform)
{
    ProgressBar::update();

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
    float healthEmptySize = healthSize * _progressBarEmptyTransform->getScale().x / 2.0f;
    screenPos.y += 4.0f;

    this->_progressBarTransform->setPos(glm::vec3(screenPos.x, screenPos.y, this->_progressBarTransform->getPos().z));
    this->_progressBarTransform->translate(-glm::vec3(healthEmptySize - ((this->_progressBarTransform->getScale().x * healthSize) / 2.0f),
                                                    0.0f, 0.0f));
    this->_progressBarEmptyTransform->setPos(glm::vec3(screenPos.x, screenPos.y, this->_progressBarEmptyTransform->getPos().z));
}
