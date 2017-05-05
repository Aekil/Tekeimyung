/*
** @Author : Julien CHARDON
*/

#include <Engine/Debug/Logger.hpp>

#include <Game/Scripts/GoldManager.hpp>

void GoldManager::start()
{
    this->_golds = STARTING_AMOUNT_OF_GOLD;
}

void GoldManager::update(float dt)
{
    static float elapsedTime = 0;

    if (this->_increaseOnTime)
    {
        elapsedTime += dt;
        if (elapsedTime >= GOLD_TIMER)
        {
            addGolds(GOLD_TIME_BONUS);
            elapsedTime = 0.0f;
        }
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

bool GoldManager::removeGolds(int removedGolds)
{
    if (this->_golds - removedGolds < 0)
        return (false);

    secureSetGolds(this->_golds - removedGolds);
    return (true);
}

void GoldManager::secureSetGolds(int futureValue)
{
    if (futureValue > MAX_AMOUNT_OF_GOLD)
    {
        this->_golds = MAX_AMOUNT_OF_GOLD;
    }
    else if (futureValue < 0)
    {
        this->_golds = 0;
    }
    else
        this->_golds = futureValue;
}

void GoldManager::setIncreaseOnTime(bool increaseOnTime)
{
    this->_increaseOnTime = increaseOnTime;
}
