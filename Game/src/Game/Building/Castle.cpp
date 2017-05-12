#include <Engine/Core/Components/Components.hh>
#include <Engine/BasicState.hpp>
#include <Engine/Utils/LevelLoader.hpp>

#include <Game/Building/Castle.hpp>
#include <Game/Character/Enemy.hpp>

void Castle::start()
{
    this->_render = getComponent<sRenderComponent>();
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_render);
    _hitCastle = EventSound::getEventByEventType(eEventSound::ENEMY_HIT_CASTLE);
}

void Castle::update(float dt)
{
    Health::update(_transform);
}

void Castle::onCollisionEnter(Entity* entity)
{
    if (entity->getTag() == ENEMY_TAG)
    {
        this->takeDamage(DEFAULT_CASTLE_DMG_FROM_ENEMY);

        sScriptComponent* script = entity->getComponent<sScriptComponent>();

        if (script == nullptr)
        {
            LOG_WARN("Could not find %s on Entity %s", "sScriptComponent", "Enemy");
            return;
        }

        Enemy* enemy = script->getScript<Enemy>(ENEMY_TAG);

        if (enemy == nullptr)
        {
            LOG_WARN("Could not find script %s on Entity %s", "Enemy", "Enemy");
            return;
        }

        enemy->remove();

#if (ENABLE_SOUND)
        if (_hitCastle->soundID != -1)
        {
            SoundManager::getInstance()->playSound(_hitCastle->soundID);
        }
#endif
    }
}

void Castle::death()
{
    //Entity* explosion = Instantiate("CASTLE_EXPLOSION");
    //sTransformComponent* explosionTransform = explosion->getComponent<sTransformComponent>();
    //explosionTransform->setPos(this->_transform->getPos());

    this->Destroy();
}

bool        Castle::updateEditor()
{
    bool    changed = false;
    int     currentHealth = this->getHealth();
    int     maxHealth = this->getMaxHealth();


    ImGui::BeginGroup();
    if (ImGui::InputInt("Current health", &currentHealth, 1, 100, ImGuiInputTextFlags_CharsNoBlank))
    {
        this->setHealth(currentHealth);
        changed |= true;
    }
    if (ImGui::InputInt("Max health", &maxHealth, 1, 100, ImGuiInputTextFlags_CharsNoBlank))
    {
        this->setMaxHealth(maxHealth);
        changed |= true;
    }
    ImGui::EndGroup();

    return (changed);
}

JsonValue   Castle::saveToJson()
{
    JsonValue   castleJson;

    castleJson.setInt("current_health", this->getHealth());
    castleJson.setInt("max_health", this->getMaxHealth());
    return (castleJson);
}

void    Castle::loadFromJson(const JsonValue& json)
{
    int currentHealth = json.getInt("current_health", 0);
    int maxHealth = json.getInt("max_health", 0);

    this->setHealth(currentHealth);
    this->setMaxHealth(maxHealth);
}
