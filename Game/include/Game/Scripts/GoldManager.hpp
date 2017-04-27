/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/BaseScript.hpp>
#include <Engine/Core/ScriptFactory.hpp>

#define GOLD_MANAGER_TAG            "GoldManager"

#define MAX_AMOUNT_OF_GOLD          1000000 // $$$ \o/ $$$
#define STARTING_AMOUNT_OF_GOLD     300

#define GOLD_TIME_BONUS             1 // amount of golds we earn every GOLD_TIMER
#define GOLD_TIMER                  0.5f // x seconds

#define WARN_STR_GOLD_LIMITS    "[GAME] Gold limits reached / Attempt to add inappropriate gold amount / Integer overflow potential"

class GoldManager final : public BaseScript
{
public:
    GoldManager() = default;
    ~GoldManager() = default;

    void    start();
    void    update(float dt);

    int     getGolds() const;

    void    setGolds(int newGoldAmount);
    void    addGolds(int addedGolds);
    bool    removeGolds(int removedGolds);
    void    setIncreaseOnTime(bool);

private:
    void    secureSetGolds(int futureValue);

private:
    int     _golds;
    bool    _increaseOnTime = true;
};

REGISTER_SCRIPT(GoldManager);