/**
* @Author   Julien CHARDON
*/

#include <Engine/EntityFactory.hpp>
#include <Game/Character/Enemy.hpp>
#include <Game/Manager/GameManager.hpp>
#include <Game/Manager/TutoManager.hpp>
#include <Game/Manager/WaveManager.hpp>
#include <Game/Manager/GoldManager.hpp>
#include <Game/Scripts/Spawner.hpp>
#include <Game/Manager/TutoManagerMessage.hpp>
#include <Game/GameStates/PlayState.hpp>

bool    TutoManager::_tutorialDone = false;

TutoManager::~TutoManager()
{
    TutoManagerMessage::_instance = nullptr;
}

eTutoState& operator++(eTutoState& state)
{
    int tmp = static_cast<int>(state);

    ++tmp;
    state = static_cast<eTutoState>(tmp);
    return (state);
}

eTutoState operator++(eTutoState& state, int)
{
    eTutoState tmp(state);
    ++state;
    return (tmp);
}

TutoManager::TutoManager()
{
    _steps = {
        {
            eTutoState::MOVE,
            "Press W, A, S or D to move (Press K to SKIP the tutorial)"
        },
        {
            eTutoState::BUILD_BASE_TOWER,
            "Press 1 and build your first tower base !\n Tower bases are used to build towers on it"
        },
        {
            eTutoState::BUILD_TOWER,
            "Press 3 and select a tower base to build a tower on it"
        },
        {
            eTutoState::DEACTIVATE_BUILD,
            "Deactivate the build zone with the mouse right click"
        },
        {
            eTutoState::ENEMY_DEAD,
            "Towers will automatically shoot on enemies"
        },
        {
            eTutoState::BUILD_WALL,
            "Try to build a wall by pressing 2\n\
Walls are very cheap and can help you to make the enemies follow a specific path"
        },
        {
            eTutoState::ENEMY_DEAD,
            ""
        },
        {
            eTutoState::SHOOT,
            "Use LEFT CLICK to shoot !"
        },
        {
            eTutoState::CHANGE_WEAPON,
            "Use the mouse SCROLL WHEEL to change your weapon !"
        },
        {
            eTutoState::ENEMY_DEAD,
            "Try to kill this enemy"
        },
        /*{
            eTutoState::CHECK_BUILDLIST,
            "Check the Build list with B to know what you can build and for how much golds"
        },*/
        {
            eTutoState::CHECK_HOWTOPLAY,
            "Check the 'How to Play' from the pause menu (press Escape) for more informations"
        },
        {
            eTutoState::TUTO_DONE,
            "Well done ! Tutorial completed.\n\
Good luck & have fun ! (press T to QUIT tutorial)"
        },
        {
            eTutoState::TUTO_WAVE,
            "Press SPACE to start the wave"
        }
    };
}

void TutoManager::start()
{
    _textComp = getComponent<sTextComponent>();

    if (_tutorialDone)
    {
        _currentState = _steps.size() - 1;
    }
    _textComp->text.setContent(_steps[_currentState].message);

    auto em = EntityFactory::getBindedEntityManager();
    auto gameManager = em->getEntityByTag("GameManager");

    if (gameManager == nullptr)
    {
        LOG_ERROR("No entity with GameManager tag");
        return;
    }

    _goldManager = getEntityScript<GoldManager>(gameManager, "GoldManager");

    if (_goldManager == nullptr)
    {
        LOG_ERROR("No GoldManager script on gameManager");
        return;
    }
    _goldManager->setIncreaseOnTime(false);

    _waveManager = getEntityScript<WaveManager>(gameManager, "WaveManager");

    if (_waveManager == nullptr)
    {
        LOG_ERROR("No WaveManager script on gameManager");
        return;
    }

}

void TutoManager::update(float dt)
{
    if (_waveManager && tutorialDone())
    {
        _waveManager->setTutorialIsFinished(true);
    }
}

void    TutoManager::sendMessage(eTutoState state)
{
    if (state == getCurrentState() && _textComp != nullptr)
    {
        ++_currentState;
        _textComp->text.setContent(_steps[_currentState].message);

        if (state == eTutoState::CHANGE_WEAPON)
            destroyBuilds();

        if (getCurrentState() == eTutoState::ENEMY_DEAD)
        {
            float speed = -1.0f;
            if (state == eTutoState::CHANGE_WEAPON)
            {
                speed = 10.0f;
            }
            spawnEnemy(speed);
        }
    }
}

bool    TutoManager::stateOnGoingOrDone(eTutoState state)
{
    uint32_t stepNb = 0;
    for (auto& step: _steps)
    {
        if (stepNb > _currentState)
            return (false);
        else if (step.state == state)
            return (true);
        ++stepNb;
    }
    return (false);
}

bool    TutoManager::stateOnGoing(eTutoState state)
{
    return (getCurrentState() == state);
}

bool    TutoManager::tutorialDone()
{
    return (getCurrentState() == eTutoState::TUTO_WAVE);
}

eTutoState  TutoManager::getCurrentState() const
{
    return (getState(_currentState));
}

/**
    This is a static function called from WaveManager,
    that is why getEntityByTag is used !
*/
void        TutoManager::display(bool displayed)
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* tutoManager = em->getEntityByTag("TutoManager");

    if (tutoManager != nullptr)
    {
        sRenderComponent*   render = tutoManager->getComponent<sRenderComponent>();

        if (render != nullptr)
            render->enabled = displayed;
    }
}

void    TutoManager::spawnEnemy(float speed)
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* spawner = em->getEntityByTag("Spawner");

    if (!spawner)
    {
        LOG_ERROR("TutoManager::spawnEnemy: Can't find Spawner entity");
        return;
    }

    Spawner* spawnerScript = getEntityScript<Spawner>(spawner, "Spawner");
    if (!spawnerScript)
    {
        LOG_ERROR("TutoManager::spawnEnemy: Spawner don't have Spawner script");
        return;
    }

    Entity* enemy = spawnerScript->spawnEntity("ENEMY");

    // Change enemy speed
    if (speed != -1.0f)
    {
        Enemy* enemyScript = getEntityScript<Enemy>(enemy, "Enemy");
        if (!enemyScript)
        {
            LOG_ERROR("TutoManager::spawnEnemy: Enemy don't have Enemy script");
            return;
        }

        enemyScript->_speed = speed;
    }
}

void    TutoManager::destroyBuilds()
{
    auto    em = EntityFactory::getBindedEntityManager();
    Entity* tower = em->getEntityByTag("Tower");
    Entity* towerBase = em->getEntityByTag("TileBaseTurret");

    // Tower explosion
    {
        Entity* explosion = Instantiate("ENEMY_EXPLOSION");
        sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
        sTransformComponent* entityTransform = entity->getComponent<sTransformComponent>();
        explosionTransform->setPos(tower->getComponent<sTransformComponent>()->getPos());
    }

    // TowerBase explosion
    {
        Entity* explosion = Instantiate("ENEMY_EXPLOSION");
        sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
        sTransformComponent* entityTransform = entity->getComponent<sTransformComponent>();
        explosionTransform->setPos(towerBase->getComponent<sTransformComponent>()->getPos());
    }

    Destroy(tower);
    Destroy(towerBase);
}

eTutoState    TutoManager::getState(uint32_t stateIndex) const
{
    if (stateIndex >= _steps.size())
        return (_steps.back().state);
    else if (stateIndex < 0)
        return (_steps[0]).state;

    return (_steps[stateIndex].state);
}
