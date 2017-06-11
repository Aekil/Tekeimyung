/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/Components/ScriptComponent.hh>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Game/Character/Enemy.hpp>
#include <Game/GameStates/PlayState.hpp>
#include <Game/Manager/GameManager.hpp>
#include <Game/Manager/GoldManager.hpp>
#include <Game/Scripts/CheatCodesScript.hpp>

#include <Game/GameStates/ConsoleState.hpp>


ConsoleState::~ConsoleState() {}

void    ConsoleState::onEnter()
{
    SoundManager::getInstance()->setVolumeAllChannels(0.25f);
}

void    ConsoleState::setupSystems()
{
    _world.addSystem<ScriptSystem>();
    _world.addSystem<UISystem>();
    _world.addSystem<RenderingSystem>(nullptr);
}

bool    ConsoleState::init()
{
    auto em = _world.getEntityManager();
    _console = em->getEntityByTag("Console");

    if (!_console)
    {
        LOG_ERROR("ConsoleState::init: Can't find entity with tag \"Console\"");
    }

    uint32_t stateNb = static_cast<uint32_t>(_gameStateManager->getStates().size());
    if (stateNb == 1)
    {
        LOG_ERROR("ConsoleState::handleCheatCodeKillAll: ConsoleState has to be on PlayState level");
        return (false);
    }

    return (true);
}

bool    ConsoleState::update(float elapsedTime)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();

    // Display the previous states
    renderPreviousStates({PlayState::identifier});

    bool success = GameState::update(elapsedTime);

    // Unpause the game
    if (keyboard.getStateMap()[Keyboard::eKey::ESCAPE] == Keyboard::eKeyState::KEY_PRESSED)
    {
        return (false);
    }
    else if (keyboard.getStateMap()[Keyboard::eKey::ENTER] == Keyboard::eKeyState::KEY_PRESSED)
    {
        if (!_console)
        {
            LOG_ERROR("ConsoleState::update: Can't enter cheat codes because the entity is not found");
            return (false);
        }

        CheatCodesScript* cheatCodeScripts = BaseScript::getEntityScript<CheatCodesScript>(_console, "CheatCodesScript");
        if (!cheatCodeScripts)
        {
            LOG_ERROR("ConsoleState::update: Can't enter cheat codes because the console entity doesn not have the script \"CheatCodesScript\"");
            return (false);
        }

        handleCheatCode(cheatCodeScripts->getText().getContent());

        return (false);
    }

    return (success);
}

void    ConsoleState::handleCheatCodeKillAll()
{
    const auto& enemies = _playStateEntityManager->getEntitiesByTag("Enemy");

    for (auto& enemy : enemies)
    {
        Enemy*  enemyScript = BaseScript::getEntityScript<Enemy>(enemy, "Enemy");

        if (!enemyScript)
        {
            LOG_ERROR("ConsoleState::handleCheatCodeKillAll: Can't find script \"Enemy\" on enemy");
            continue;
        }

        enemyScript->death();
    }
}

void    ConsoleState::handleCheatCodeGiveMeGold()
{
    Entity* gameManager = _playStateEntityManager->getEntityByTag(GAME_MANAGER_TAG);

    if (!gameManager)
    {
        LOG_ERROR("ConsoleState::handleCheatCodeGiveMeGold: Can't find entity with tag \"%s\"", GAME_MANAGER_TAG);
        return;
    }

    GoldManager* goldManager = BaseScript::getEntityScript<GoldManager>(gameManager, GOLD_MANAGER_TAG);

    if (!goldManager)
    {
        LOG_ERROR("ConsoleState::handleCheatCodeGiveMeGold: Can't find script \"%s\" on gameManager", GOLD_MANAGER_TAG);
        return;
    }

    goldManager->addGolds(1000);
}

void    ConsoleState::handleCheatCode(const std::string& cheatCode)
{
    uint32_t stateNb = static_cast<uint32_t>(_gameStateManager->getStates().size());
    _playStateEntityManager = _gameStateManager->getStates()[stateNb - 2]->getWorld().getEntityManager();

    EntityFactory::bindEntityManager(_playStateEntityManager);

    if (cheatCode == "killall")
    {
        handleCheatCodeKillAll();
    }
    else if (cheatCode == "give me gold")
    {
        handleCheatCodeGiveMeGold();
    }

    EntityFactory::bindEntityManager(_world.getEntityManager());
}
