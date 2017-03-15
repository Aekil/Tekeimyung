/*
** @Author : Simon AMBROISE
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Scripts/ProgressBar.hpp>

ProgressBar::~ProgressBar()
{
    auto em = EntityFactory::getBindedEntityManager();

    em->destroyEntityRegister(_progressBarEmpty);
    em->destroyEntityRegister(_progressBar);
}

void    ProgressBar::init(const std::string& entityProgressBarEmpty, const std::string& entityProgressBar)
{
    this->_progressBarEmpty = EntityFactory::createEntity(entityProgressBarEmpty);
    this->_progressBar = EntityFactory::createEntity(entityProgressBar);
    this->_progressBarEmptyTransform = _progressBarEmpty->getComponent<sTransformComponent>();
    this->_progressBarTransform = _progressBar->getComponent<sTransformComponent>();
}

void ProgressBar::update()
{
    this->_progressBarTransform->setScale(glm::vec3((float)_currentProgress / (float)_maxProgress * _progressBarEmptyTransform->getScale().x,
                                                    this->_progressBarTransform->getScale().y,
                                                    this->_progressBarTransform->getScale().z));
}
