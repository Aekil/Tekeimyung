#include <Engine/Components.hh>
#include <Engine/BasicState.hpp>
#include <Engine/Utils/LevelLoader.hpp>

#include <Game/Scripts/Enemy.hpp>
#include <Game/Scripts/Castle.hpp>

void Castle::start()
{
    this->setHealth(MAX_CASTLE_HEALTH);
    this->setMaxHealth(MAX_CASTLE_HEALTH);

    this->_render = getComponent<sRenderComponent>();
    this->_transform = getComponent<sTransformComponent>();
    Health::init(_render);
}

void Castle::update(float dt)
{
    Health::update(_transform);
}

void Castle::onCollisionEnter(Entity* entity)
{
    if (entity->getTag() == ENEMY_TAG)
    {
        this->takeDamage(DMG_FOR_EACH_ENEMY);

        sScriptComponent* script = entity->getComponent<sScriptComponent>();
        Enemy* enemy = script ? script->getScript<Enemy>(ENEMY_TAG) : nullptr;
        enemy->remove();
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
        this->setMaxHealth(currentHealth);
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