/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Manager/GameManager.hpp>
#include <Game/Manager/WaveManager.hpp>

#include <Game/Hud/DisplayWave.hpp>

void DisplayWave::start()
{
    this->_em = EntityFactory::getBindedEntityManager();
    if (this->_em != nullptr)
    {
        this->_gameManager = this->_em->getEntityByTag(GAME_MANAGER_TAG);
        if (this->_gameManager != nullptr)
            this->_waveHudDisplay = this->_em->getEntityByTag(HUD_DISPLAY_WAVE_TAG);
    }
}

void DisplayWave::update(float dt)
{
    if (this->_gameManager != nullptr && this->_waveHudDisplay != nullptr)
    {
        sScriptComponent*   scriptComp = _gameManager->getComponent<sScriptComponent>();
        WaveManager*        waveManager = scriptComp->getScript<WaveManager>(WAVE_MANAGER_TAG);
        int                 nbWaves = waveManager->getNbWaves();
        int                 currentWave = waveManager->getCurrentWave();

        if (nbWaves != this->_tmpNbWaves || currentWave != this->_tmpcurrentWave)
        {
            sTextComponent*     textComp = _waveHudDisplay->getComponent<sTextComponent>();
            char                waveText[MAX_SIZE_TEXT_WAVES];

            sprintf_s(waveText, "%d/%d", currentWave, nbWaves);
            textComp->text.setContent(waveText);
            this->_tmpNbWaves = nbWaves;
            this->_tmpcurrentWave = currentWave;
        }
    }
}
