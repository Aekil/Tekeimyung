/*
** @Author : Julien CHARDON
*/

#pragma once

#include <Engine/Core/ScriptFactory.hpp>

#define MAX_AMOUNT_OF_GOLD          1000000 // $$$ \o/ $$$
#define STARTING_AMOUNT_OF_GOLD     200

#define WARN_STR_GOLD_LIMITS    "[GAME] Gold limits reached / Attempt to add inappropriate gold amount / Integer overflow potential"

class Gold final : public BaseScript
{
public:
    Gold() = default;
    ~Gold() = default;

    void    start();
    void    update(float dt);

    int     getGolds() const;

    void    setGolds(int newGoldAmount);
    void    addGolds(int addedGolds);
    bool    removeGolds(int removedGolds);

private:
    void    secureSetGolds(int futureValue);

private:
    int     _golds;
};

REGISTER_SCRIPT(Gold);