/*
** @Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/ProgressBar.hpp>

ProgressBar::~ProgressBar()
{
    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(empty);
    em->destroyEntityRegister(filled);
}

void    ProgressBar::init(const std::string& entityProgressBarEmpty, const std::string& entityProgressBar)
{
    this->empty = EntityFactory::createEntity(entityProgressBarEmpty);
    this->filled = EntityFactory::createEntity(entityProgressBar);
    this->emptyTransform = empty->getComponent<sTransformComponent>();
    this->filledTransform = filled->getComponent<sTransformComponent>();
}

void ProgressBar::update()
{
    this->filledTransform->setScale(glm::vec3((float)currentProgress / (float)maxProgress * emptyTransform->getScale().x,
                                                    this->filledTransform->getScale().y,
                                                    this->filledTransform->getScale().z));
}
