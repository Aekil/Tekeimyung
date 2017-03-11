/*
** @Author : Julien CHARDON
*/

#include <Engine/Utils/Logger.hpp>

#include <Game/Scripts/Gold.hpp>

void Gold::start()
{
    _golds = STARTING_AMOUNT_OF_GOLD;
}

void Gold::update(float dt)
{

}

inline int Gold::getGolds() const
{
    return (_golds);
}

inline void Gold::setGolds(int newGoldAmount)
{
    secureSetGolds(newGoldAmount);
}

inline void Gold::addGolds(int addedGolds)
{
    secureSetGolds(_golds + addedGolds);
}

inline bool Gold::removeGolds(int removedGolds)
{
    secureSetGolds(_golds - removedGolds);
}

void Gold::secureSetGolds(int futureValue)
{
    if (futureValue > MAX_AMOUNT_OF_GOLD)
    {
        _golds = MAX_AMOUNT_OF_GOLD;
        LOG_WARN(WARN_STR_GOLD_LIMITS);
    }
    else if (futureValue < 0)
    {
        _golds = 0;
        LOG_WARN(WARN_STR_GOLD_LIMITS);
    }
    else
        _golds = futureValue;
}