/*
** @Author : Simon AMBROISE
*/

#include <Game/Scripts/LifeTime.hpp>

void LifeTime::start() {}

void LifeTime::update(float dt)
{
    if (this->_lifeTime == 0.0f)
        return;

    this->_elapsedTime += dt;

    if (this->_elapsedTime > this->_lifeTime)
        this->Destroy();
}

bool LifeTime::updateEditor()
{
    bool changed = false;

    ImGui::Text("Lifetime for entity (0 is disabled)");

    changed |= ImGui::InputFloat("LifeTime", &(this->_lifeTime), 0.0f, ImGuiInputTextFlags_AllowTabInput);
    
    return (changed);
}

JsonValue LifeTime::saveToJson()
{
    JsonValue json;
    
    json.setFloat("lifetime", this->_lifeTime);
    return (json);
}

void LifeTime::loadFromJson(const JsonValue& json)
{
    this->_lifeTime = json.getFloat("lifetime", 0.0f);
}