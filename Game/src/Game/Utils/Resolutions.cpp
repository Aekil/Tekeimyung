#include <Game/Utils/Resolutions.hpp>

std::shared_ptr<Resolutions> Resolutions::_instance;

Resolutions::Resolutions()
{
    this->_resolutionsMap["onCollisionEnterPlayer"] = Resolutions::onCollisionEnterPlayer;
    this->_resolutionsMap["onCollisionExitPlayer"] = Resolutions::onCollisionExitPlayer;
}

std::function<void(int)> Resolutions::getResolutionByName(std::string name)
{
    return (this->_resolutionsMap[name]);
}

void Resolutions::onCollisionEnterPlayer(int entityId)
{
    LOG_DEBUG("coucou %d", entityId);
}

void Resolutions::onCollisionExitPlayer(int entityId)
{
    LOG_DEBUG("anrevoir %d", entityId);
}

std::shared_ptr<Resolutions> Resolutions::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<Resolutions>();

    return (_instance);
}