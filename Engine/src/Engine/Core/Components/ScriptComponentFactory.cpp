/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/Components/ScriptComponentFactory.hpp>

constexpr unsigned int sScriptComponent::identifier;

sComponent* ComponentFactory<sScriptComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sScriptComponent*  component = new sScriptComponent();

    auto    scripts = json.get()["scripts"];
    if (scripts.size() > 0 && scripts.type() != Json::ValueType::arrayValue)
    {
        LOG_ERROR("%s::sScriptComponent loadFromJson error: 'scripts' is not an array", entityType.c_str());
        return (component);
    }

    for (const auto& script: scripts)
    {
        JsonValue   scriptJson(script);
        std::string scriptName = scriptJson.getString("name", "");
        auto        scriptInstance = ScriptFactory::create(scriptName);

        if (scriptInstance == nullptr)
        {
            LOG_ERROR("%s::sScriptComponent loadFromJson error: script \"%s\" does not exist", entityType.c_str(), scriptName.c_str());
            continue;
        }

        LOG_DEBUG("Loading script '%s' from Json...", scriptName.c_str());
        scriptInstance->loadFromJson(scriptJson);

        component->scripts.push_back(std::move(scriptInstance));
    }

    return component;
}

JsonValue&    ComponentFactory<sScriptComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sScriptComponent* component = static_cast<const sScriptComponent*>(savedComponent ? savedComponent : _components[entityType]);

    std::vector<JsonValue> scriptsJson;
    for (const auto& script: component->scripts)
    {
        JsonValue scriptJson = script->saveToJson();
        scriptJson.setString("name", script->getName());
        scriptsJson.push_back(scriptJson);
    }


    json.setValueVec("scripts", scriptsJson);

    return (json);
}

bool    ComponentFactory<sScriptComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sScriptComponent* component = static_cast<sScriptComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    ImGui::Separator();
    ImGui::Text("Scripts");
    ImGui::SameLine();

    //  Add new script button
    if (ImGui::Button("Add a script"))
    {
        ImGui::OpenPopup("scripts");
    }

    //  Display new scripts that can be added
    if (ImGui::BeginPopup("scripts"))
    {
        for (auto script: ScriptFactory::getScriptsNames())
        {
            //  Entity does not have this script
            if (!component->hasScript(script))
            {
                //  Script button pressed, add it to the entity
                if (ImGui::Button(script))
                {
                    auto scriptInstance = ScriptFactory::create(script);
                    scriptInstance->setEntity(entity);
                    component->scripts.push_back(std::move(scriptInstance));
                    //return (true);
                }
            }
        }
        ImGui::EndPopup();
    }

    if (component->selectedScript != nullptr)
    {
        //  Delete script
        ImGui::SameLine();
        //ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.6f, 0.0f, 0.0f, 0.5f));
        //ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(0.6f, 0.0f, 0.0f, 1.0f));
        if (ImGui::Button("Delete"))
        {
            uint32_t    index = 0;

            //  Find script index in scripts vector
            for (const auto& script : component->scripts)
            {
                if (script.get() == component->selectedScript)
                    break;
                index++;
            }
            component->scripts.erase(component->scripts.begin() + index);
            component->selectedScript = nullptr;
            return (true);
        }
        //ImGui::PopStyleColor(2);
    }

    // Scripts list
    ImGui::BeginChild("Scripts", ImVec2(0, 100), true);
    for (auto& script: component->scripts)
    {
        if (ImGui::Selectable(script->getName().c_str(), component->selectedScript == script.get()))
        {
            component->selectedScript = script.get();
        }
    }
    ImGui::EndChild();

    if (component->selectedScript != nullptr)
    {
        ImGui::BeginGroup();
        ImGui::Text("%s script options", component->selectedScript->getName().c_str());

        component->selectedScript->updateEditor();
        ImGui::EndGroup();
    }

    return (changed);
}
