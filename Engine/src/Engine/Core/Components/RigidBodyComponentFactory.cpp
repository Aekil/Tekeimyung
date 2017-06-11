/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>

#include <Engine/Debug/Logger.hpp>

#include <Engine/Core/Components/RigidBodyComponentFactory.hpp>

constexpr unsigned int sRigidBodyComponent::identifier;

sComponent* ComponentFactory<sRigidBodyComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sRigidBodyComponent* component = new sRigidBodyComponent();

    component->gravity = json.getVec3f("gravity", {0.0f, 0.0f, 0.0f});
    component->velocity = json.getVec3f("velocity", {0.0f, 0.0f, 0.0f});
    component->collisionsEnabled = json.getBool("collisions_enabled", true);

    auto ignoredTags = json.get()["ignored_tags"];
    if (ignoredTags.size() > 0 && ignoredTags.type() != Json::ValueType::arrayValue)
    {
        LOG_ERROR("%s::sRigidBodyComponent loadFromJson error: ignored tags is not an array", "ignoredTags");
        return (nullptr);
    }

    for (const auto& ignoredTag : ignoredTags)
    {
        JsonValue ignoredTagJson(ignoredTag);

        std::string ignoredTagString = ignoredTagJson.getString("tag", "");
        component->ignoredTags.push_back(ignoredTagString);
    }

    return (component);
}

JsonValue&    ComponentFactory<sRigidBodyComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sRigidBodyComponent* component = static_cast<const sRigidBodyComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("gravity", component->gravity);
    json.setVec3f("velocity", component->velocity);
    json.setBool("collisions_enabled", component->collisionsEnabled);

    std::vector<JsonValue> jsonIgnoredTags;

    for (auto& ignoredTag : component->ignoredTags)
    {
        JsonValue jsonIgnoredTag;

        jsonIgnoredTag.setString("tag", ignoredTag);
        jsonIgnoredTags.push_back(jsonIgnoredTag);
    }

    json.setValueVec("ignored_tags", jsonIgnoredTags);

    return (json);
}

bool    ComponentFactory<sRigidBodyComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sRigidBodyComponent* component = static_cast<sRigidBodyComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    if (ImGui::Button(component->collisionsEnabled ? "Disabled" : "Enabled"))
    {
        component->collisionsEnabled = !component->collisionsEnabled;
    }

    ImGui::InputFloat3("gravity", glm::value_ptr(component->gravity), 3);
    ImGui::InputFloat3("velocity", glm::value_ptr(component->velocity), 3);

    ImGui::Text("Ignored tags (for collision)");

    // Create new config
    {
        if (ImGui::Button("New tag"))
        {
            component->ignoredTags.push_back("Default");
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete tag"))
    {
        component->ignoredTags.erase(component->ignoredTags.begin() + component->selectedTags);
        if (component->selectedTags >= component->ignoredTags.size())
            component->selectedTags--;
    }

    ImGui::BeginChild("Tags", ImVec2(0, 100), true);
    uint32_t i = 0;
    for (auto& ignoredTag : component->ignoredTags)
    {
        ImGui::PushID(i);
        if (ImGui::Selectable(ignoredTag.c_str(), component->selectedTags == i))
        {
            component->selectedTags = i;
        }
        i++;
        ImGui::PopID();
    }
    ImGui::EndChild();

    if (component->selectedTags == -1)
        return (false);

    // Edit tag name
    {
        ImGui::Text("\n");
        const std::string& tagName = component->ignoredTags[component->selectedTags];
        std::vector<char> tagNameVec(tagName.cbegin(), tagName.cend());
        tagNameVec.push_back(0);
        tagNameVec.resize(64);

        if (ImGui::InputText("Name", tagNameVec.data(), tagNameVec.size()))
        {
            component->ignoredTags[component->selectedTags] = (tagNameVec.data());
        }
    }

    return (changed);
}
