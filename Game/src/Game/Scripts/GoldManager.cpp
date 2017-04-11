/*
** @Author : Julien CHARDON
*/

#include <Engine/Utils/Logger.hpp>

#include <Game/Scripts/GoldManager.hpp>

void GoldManager::start()
{
    this->_golds = STARTING_AMOUNT_OF_GOLD;
    LOG_DEBUG("%d", this->_golds);
}

void GoldManager::update(float dt)
{
    static float elapsedTime = 0;

    elapsedTime += dt;
    if (elapsedTime >= GOLD_TIMER)
    {
        addGolds(GOLD_TIME_BONUS);
        elapsedTime -= GOLD_TIMER;
    }
}

int GoldManager::getGolds() const
{
    return (this->_golds);
}

void GoldManager::setGolds(int newGoldAmount)
{
    secureSetGolds(newGoldAmount);
}

void GoldManager::addGolds(int addedGolds)
{
    secureSetGolds(this->_golds + addedGolds);
}

void GoldManager::removeGolds(int removedGolds)
{
    secureSetGolds(this->_golds - removedGolds);
}

void GoldManager::secureSetGolds(int futureValue)
{
    if (futureValue > MAX_AMOUNT_OF_GOLD)
    {
        this->_golds = MAX_AMOUNT_OF_GOLD;
        LOG_WARN(WARN_STR_GOLD_LIMITS);
    }
    else if (futureValue < 0)
    {
        this->_golds = 0;
        LOG_WARN(WARN_STR_GOLD_LIMITS);
    }
    else
        this->_golds = futureValue;
}