/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Components/UiComponent.hh>
#include <Engine/Graphics/UI/Font.hpp>
#include <Engine/Utils/ResourceManager.hpp>

#include <Engine/Core/Components/TextComponentFactory.hpp>

constexpr unsigned int sTextComponent::identifier;
DECLARE_ENUM_MANAGER(eHorizontalAlignment)
DECLARE_ENUM_MANAGER(eVerticalAlignment)

sComponent* ComponentFactory<sTextComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTextComponent*  component;

    component = new sTextComponent();

    component->text.setContent(json.getString("content", ""));
    component->text.setColor(json.getColor4f("color", {1.0f, 1.0f, 1.0f, 1.0f}));
    component->text.setFontSize(json.getUInt("font_size", 10));
    component->horizontalAlignment = EnumManager<eHorizontalAlignment>::stringToEnum(json.getString("horizontal_alignment", "MIDDLE"));
    component->verticalAlignment = EnumManager<eVerticalAlignment>::stringToEnum(json.getString("vertical_alignment", "MIDDLE"));
    component->offset = json.getVec2f("offset", {});

    // Load text font
    {
        std::string fontName = json.getString("font_name", "arial.ttf");
        Font* font = ResourceManager::getInstance()->getResource<Font>(fontName);

        if (!font)
        {
            EXCEPT(InternalErrorException, "Failed to get font resource \"%s\"", fontName.c_str());
        }

        component->text.setFont(font);
    }

    return component;
}

JsonValue&    ComponentFactory<sTextComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sTextComponent* component = static_cast<const sTextComponent*>(savedComponent ? savedComponent : _components[entityType]);


    json.setString("content", component->text.getContent());
    json.setColor4f("color", component->text.getColor());
    json.setUInt("font_size", component->text.getFontSize());
    json.setString("horizontal_alignment", EnumManager<eHorizontalAlignment>::enumToString(component->horizontalAlignment));
    json.setString("vertical_alignment", EnumManager<eVerticalAlignment>::enumToString(component->verticalAlignment));
    json.setVec2f("offset", component->offset);

    if (component->text.getFont())
    {
        json.setString("font_name", component->text.getFont()->getId());
    }

    return (json);
}

bool    ComponentFactory<sTextComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sTextComponent* component = static_cast<sTextComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    // Edit text content
    {
        std::vector<char> textVec(component->text.getContent().cbegin(), component->text.getContent().cend());
        textVec.push_back(0);
        textVec.resize(64);

        if (ImGui::InputTextMultiline("Text", textVec.data(), textVec.size()))
        {
            component->text.setContent(textVec.data());
            changed = true;
        }
    }

    // Edit text size
    {
        int textSize = component->text.getFontSize();
        if (ImGui::InputInt("Font size", &textSize))
        {
            textSize = std::max(textSize, 0);
            textSize = std::min(textSize, 200);
            component->text.setFontSize(textSize);
            changed = true;
        }
    }

    // Edit text color
    {
        glm::vec4 color = component->text.getColor();
        if (ImGui::ColorEdit4("Color", glm::value_ptr(color)))
        {
            component->text.setColor(color);
        }
    }

    // Edit font name
    {
        std::string fontName = component->text.getFont()->getId();
        if (Helper::updateComboString("Font name", ResourceManager::getInstance()->getResourcesNames<Font>(), fontName))
        {
            Font* font = ResourceManager::getInstance()->getResource<Font>(fontName);

            ASSERT(font != nullptr, "The font \"%s\" should be loaded", fontName.c_str());

            component->text.setFont(font);
            changed = true;
        }
    }

    // Edit alignment
    {
        changed |= Helper::updateComboEnum<eHorizontalAlignment>("Horizontal alignment", component->horizontalAlignment);
        changed |= Helper::updateComboEnum<eVerticalAlignment>("Vertical alignment", component->verticalAlignment);
    }

    // Edit offset
    {
        changed |= ImGui::InputFloat("Horizontal offset", &component->offset.x, 1.0f, ImGuiInputTextFlags_AllowTabInput);
        changed |= ImGui::InputFloat("Vertical offset", &component->offset.y, 1.0f, ImGuiInputTextFlags_AllowTabInput);
    }

    if (changed)
    {
        sUiComponent* uiComp = component->entity->getComponent<sUiComponent>();
        if (uiComp)
        {
            uiComp->needUpdate = true;
        }
    }

    return (changed);
}
