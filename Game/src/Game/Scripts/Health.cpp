/*
** @Author : Simon AMBROISE
*/

#include <glm/gtc/matrix_transform.hpp>

#include <Engine/Core/Components/Components.hh>
#include <Engine/EntityFactory.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Game/Scripts/Health.hpp>

bool Health::takeDamage(double damage)
{
    this->_progressBar.currentProgress -= damage;

    if (this->_progressBar.currentProgress <= 0)
    {
        this->death();
        return (true);
    }

    return (false);
}

int Health::getHealth() const
{
    return ((int)_progressBar.currentProgress);
}

int Health::getMaxHealth() const
{
    return ((int)_progressBar.maxProgress);
}

void    Health::setHealth(int health)
{
    _progressBar.currentProgress = (float)health;
}

void    Health::setMaxHealth(int maxHealth)
{
    _progressBar.maxProgress = (float)maxHealth;
}

void Health::init(sRenderComponent* render)
{
    _progressBar.init("HEALTH_EMPTY", "HEALTH");
    _objCenter.x = render->getModel()->getMin().x + ((render->getModel()->getMax().x - render->getModel()->getMin().x) / 2.0f);
    _objCenter.y = render->getModel()->getMax().y;
    _objCenter.z = render->getModel()->getMin().z + ((render->getModel()->getMax().z - render->getModel()->getMin().z) / 2.0f);
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

    screenPos.y += 4.0f;

    this->_progressBar.filledTransform->setPos(glm::vec3(screenPos.x, screenPos.y, this->_progressBar.filledTransform->getPos().z));
    this->_progressBar.emptyTransform->setPos(glm::vec3(screenPos.x, screenPos.y, this->_progressBar.emptyTransform->getPos().z));

    _progressBar.update();
}
