/*
** @Author : Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>

#include <Game/Scripts/WaveManager.hpp>

#include <Game/Scripts/HudDisplayWave.hpp>

void HudDisplayWave::start()
{
}

void HudDisplayWave::update(float dt)
{
    // should be update only when we change wave number
    EntityManager* em = EntityFactory::getBindedEntityManager();

    const auto& waveDisplay = em->getEntityByTag("HudDisplayWave");
    const auto& gameManager = em->getEntityByTag("GameManager");
    sScriptComponent* scriptComp = gameManager->getComponent<sScriptComponent>();
    WaveManager* waveManager = scriptComp->getScript<WaveManager>("WaveManager");
    int nbWaves = waveManager->getNbWaves();
    int currentWave = waveManager->getCurrentWave();

    sTextComponent* textComp = waveDisplay->getComponent<sTextComponent>();
    char waveText[20];
    sprintf_s(waveText, "%d/%d waves", currentWave + 1, nbWaves);
    textComp->text.setContent(waveText);
}
