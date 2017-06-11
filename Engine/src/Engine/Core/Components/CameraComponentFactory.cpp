/**
* @Author   Guillaume Labey
*/

#include <Engine/Core/Components/CameraComponentFactory.hpp>

sComponent* ComponentFactory<sCameraComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sCameraComponent*  component;

    component = new sCameraComponent();

    // Viewport
    {
        Camera::sViewport viewportRect = component->camera.getViewportRect();
        JsonValue viewportJson(json.get()["viewport"]);
        JsonValue offsetJson(viewportJson.get()["offset"]);
        JsonValue extentJson(viewportJson.get()["extent"]);

        viewportRect.offset.x = offsetJson.getFloat("x", 0.0f);
        viewportRect.offset.y = offsetJson.getFloat("y", 0.0f);
        viewportRect.extent.width = extentJson.getFloat("width", 1.0f);
        viewportRect.extent.height = extentJson.getFloat("height", 1.0f);

        component->camera.setViewportRect(viewportRect);
    }

    component->camera.setProjType(EnumManager<Camera::eProj>::stringToEnum(json.getString("projection", "PERSPECTIVE")));
    component->camera.setProjSize(json.getFloat("projection_size", 500.0f));
    component->camera.setNear(json.getFloat("near", 0.1f));
    component->camera.setFar(json.getFloat("far", 100.0f));
    component->camera.setFov(json.getFloat("fov", 45.0f));

    return component;
}

JsonValue&    ComponentFactory<sCameraComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    JsonValue viewportJson;
    const sCameraComponent* component = static_cast<const sCameraComponent*>(savedComponent ? savedComponent : _components[entityType]);

    // Viewport
    {
        Camera::sViewport viewportRect = component->camera.getViewportRect();
        JsonValue offsetJson;
        JsonValue extentJson;
        offsetJson.setFloat("x", viewportRect.offset.x);
        offsetJson.setFloat("y", viewportRect.offset.y);
        extentJson.setFloat("width", viewportRect.extent.width);
        extentJson.setFloat("height", viewportRect.extent.height);

        viewportJson.setValue("offset", offsetJson);
        viewportJson.setValue("extent", extentJson);
    }

    json.setValue("viewport", viewportJson);
    json.setString("projection", EnumManager<Camera::eProj>::enumToString(component->camera.getProjType()));
    json.setFloat("projection_size", component->camera.getProjSize());
    json.setFloat("near", component->camera.getNear());
    json.setFloat("far", component->camera.getFar());
    json.setFloat("fov", component->camera.getFov());

    return (json);
}

bool    ComponentFactory<sCameraComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sCameraComponent* component = static_cast<sCameraComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;
    bool projectionChanged = false;

    // Viewport
    {
        bool viewportChanged = false;
        Camera::sViewport viewportRect = component->camera.getViewportRect();

        ImGui::Text("Viewport");
        ImGui::PushItemWidth(100);
        viewportChanged |= ImGui::InputFloat("X", &viewportRect.offset.x, 0.1f, ImGuiInputTextFlags_AllowTabInput);
        ImGui::SameLine();
        viewportChanged |= ImGui::InputFloat("Y", &viewportRect.offset.y, 0.1f, ImGuiInputTextFlags_AllowTabInput);
        viewportChanged |= ImGui::InputFloat("Width", &viewportRect.extent.width, 0.1f, ImGuiInputTextFlags_AllowTabInput);
        ImGui::SameLine();
        viewportChanged |= ImGui::InputFloat("Height", &viewportRect.extent.height, 0.1f, ImGuiInputTextFlags_AllowTabInput);
        ImGui::PopItemWidth();

        Helper::limitValueInRange<float>(0.0f, 1.0f, viewportRect.offset.x);
        Helper::limitValueInRange<float>(0.0f, 1.0f, viewportRect.offset.y);
        Helper::limitValueInRange<float>(0.1f, 1.0f, viewportRect.extent.width);
        Helper::limitValueInRange<float>(0.1f, 1.0f, viewportRect.extent.height);

        if (viewportChanged)
        {
            changed = true;
            component->camera.setViewportRect(viewportRect);
        }
    }

    // Projection type
    {
        ImGui::Text("\n");
        Camera::eProj projection = component->camera.getProjType();
        if (Helper::updateComboEnum<Camera::eProj>("Projection", projection))
        {
            component->camera.setProjType(projection);
            changed = true;
            projectionChanged = true;
        }
    }

    // Near
    {
        float near = component->camera.getNear();
        if (ImGui::InputFloat("Near", &near, 5.0f, ImGuiInputTextFlags_AllowTabInput))
        {
            component->camera.setNear(near);
            changed = true;
        }
    }

    // Far
    {
        float far = component->camera.getFar();
        if (ImGui::InputFloat("Far", &far, 5.0f, ImGuiInputTextFlags_AllowTabInput))
        {
            component->camera.setFar(far);
            changed = true;
        }
    }

    // Projection size
    {
        // Fov is only used for perspective
        if (component->camera.getProjType() == Camera::eProj::PERSPECTIVE)
        {
            float fov = component->camera.getFov();
            if (ImGui::InputFloat("Fov", &fov, 1.0f, ImGuiInputTextFlags_AllowTabInput))
            {
                component->camera.setFov(fov);
                changed = true;
            }
        }
        else if (component->camera.getProjType() == Camera::eProj::ORTHOGRAPHIC_3D)
        {
            float projSize = component->camera.getProjSize();
            if (ImGui::InputFloat("Projection size", &projSize, 10.0f, ImGuiInputTextFlags_AllowTabInput))
            {
                component->camera.setProjSize(projSize);
                changed = true;
            }
        }
    }

    // In perspective we need to recreate the trapeze model
    if (projectionChanged ||
        (changed && component->camera.getProjType() == Camera::eProj::PERSPECTIVE))
    {
        component->_cameraView = nullptr;
    }


    return (changed);
}
