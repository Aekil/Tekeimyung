/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/WaveManager.hpp>

#include <Game/Scripts/HudDisplayWave.hpp>

void HudDisplayWave::start()
{
    _em = EntityFactory::getBindedEntityManager();
    if (_em != nullptr)
    {
        _gameManager = _em->getEntityByTag(GAME_MANAGER_TAG);
        if (_gameManager != nullptr)
            _waveHudDisplay = _em->getEntityByTag(HUD_DISPLAY_WAVE_TAG);
    }
}

void HudDisplayWave::update(float dt)
{
    if (_gameManager != nullptr && _waveHudDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = _gameManager->getComponent<sScriptComponent>();
        WaveManager*        waveManager = scriptComp->getScript<WaveManager>(WAVE_MANAGER_TAG);
        sTextComponent*     textComp = _waveHudDisplay->getComponent<sTextComponent>();
        char                waveText[MAX_SIZE_TEXT_WAVES];
        int                 nbWaves = waveManager->getNbWaves();
        int                 currentWave = waveManager->getCurrentWave();

        sprintf_s(waveText, "%d/%d waves", currentWave, nbWaves);
        textComp->text.setContent(waveText);
    }
}
