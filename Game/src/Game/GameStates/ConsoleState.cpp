/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/Components/ScriptComponent.hh>
#include <Engine/Sound/SoundManager.hpp>
#include <Engine/Systems/RenderingSystem.hpp>
#include <Engine/Systems/ScriptSystem.hpp>
#include <Engine/Systems/UISystem.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Game/Character/Enemy.hpp>
#include <Game/Character/NewBuild.hpp>
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

void    ConsoleState::handleCheatCodeBuildForMe()
{
    NewBuild* newBuild = nullptr;
    GameManager* gameManager = nullptr;

    // Get NewBuild script
    {
        Entity* player = _playStateEntityManager->getEntityByTag("Player");

        if (!player)
        {
            LOG_ERROR("ConsoleState::handleCheatCodeBuildForMe: Can't find entity with tag \"Player\"");
            return;
        }

        newBuild = BaseScript::getEntityScript<NewBuild>(player, "NewBuild");

        if (!newBuild)
        {
            LOG_ERROR("ConsoleState::handleCheatCodeBuildForMe: Can't find script \"NewBuild\" on player");
            return;
        }
    }

    // Get GameManager script
    {
        Entity* gameManagerEntity = _playStateEntityManager->getEntityByTag(GAME_MANAGER_TAG);

        if (!gameManagerEntity)
        {
            LOG_ERROR("ConsoleState::handleCheatCodeBuildForMe: Can't find entity with tag \"%s\"", GAME_MANAGER_TAG);
            return;
        }

        gameManager = BaseScript::getEntityScript<GameManager>(gameManagerEntity, GAME_MANAGER_TAG);

        if (!gameManager)
        {
            LOG_ERROR("ConsoleState::handleCheatCodeBuildForMe: Can't find script \"%s\" on gameManager", GAME_MANAGER_TAG);
            return;
        }
    }

    bool canBuild = true;
    while (canBuild && !gameManager->map.isFull())
    {
        Entity* tile = nullptr;
        bool found = false;
        glm::ivec2 pos;
        while (!found)
        {
            pos.x = Helper::randInt(0, gameManager->map.getWidth() - 1);
            pos.y = Helper::randInt(0, gameManager->map.getHeight() - 1);

            if (gameManager->map.isWalkable(pos.x, pos.y))
            {
                found = true;
                tile = gameManager->map.getEntities()[pos.x][pos.y];
            }
        }

        Entity* lastBuiltBaseTurret = _playStateEntityManager->getEntity(_lastBuiltBaseTurretHandle);

        // A base turret has been built previously
        // But the tower could not be built (Out of money)
        if (!lastBuiltBaseTurret)
        {
            // Build base turret
            newBuild->setCurrentChoice("TILE_BASE_TURRET");
            newBuild->setTileHovered(tile);
            // Build and get the created entity
            lastBuiltBaseTurret = newBuild->placePreviewedEntity();
            newBuild->setCurrentChoice("");
        }

        // Build tower if base turret has been built
        if (lastBuiltBaseTurret)
        {
            newBuild->setCurrentChoice("TOWER_FIRE");
            newBuild->setTileHovered(lastBuiltBaseTurret);
            canBuild = newBuild->placePreviewedEntity() != nullptr;
            newBuild->setCurrentChoice("");

            if (!canBuild)
            {
                _lastBuiltBaseTurretHandle = lastBuiltBaseTurret->handle;
            }
        }
        else
        {
            canBuild = false;
        }

        if (canBuild)
        {
            _lastBuiltBaseTurretHandle = 0;
        }
    }
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
    else if (cheatCode == "build for me")
    {
        handleCheatCodeBuildForMe();
    }

    EntityFactory::bindEntityManager(_world.getEntityManager());
}
