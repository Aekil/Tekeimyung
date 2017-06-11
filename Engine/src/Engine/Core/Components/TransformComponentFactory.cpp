/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include <Engine/Debug/Logger.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Window/GameWindow.hpp>

#include <Engine/Core/Components/TransformComponentFactory.hpp>

bool ComponentFactory<sTransformComponent>::enableGuizmos = true;

sComponent* ComponentFactory<sTransformComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTransformComponent*   component;

    component = new sTransformComponent();

    component->setScale(json.getVec3f("scale", { 1.0f, 1.0f, 1.0f }));
    component->setPos(json.getVec3f("pos", { 0.0f, 0.0f, 0.0f }));
    component->setRotation(json.getVec3f("rotation", { 0.0f, 0.0f, 0.0f }));

    return (component);
}

JsonValue&    ComponentFactory<sTransformComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sTransformComponent* component = static_cast<const sTransformComponent*>(savedComponent ? savedComponent : _components[entityType]);


    json.setVec3f("scale", component->getScale());
    json.setVec3f("pos", component->getPos());
    json.setVec3f("rotation", component->getRotation());

    return (json);
}

bool    ComponentFactory<sTransformComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sTransformComponent* component = static_cast<sTransformComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    component->translate(component->_posOffsetLocal, Transform::eTransform::LOCAL);
    component->translate(component->_posOffsetWorld, Transform::eTransform::WORLD);
    glm::vec3 pos = component->getPos();
    glm::vec3 scale = component->getScale();
    glm::vec3 rotation = component->getRotation();
    glm::mat4 transform = component->getTransform();

    if (ComponentFactory<sTransformComponent>::updateTransforms(pos,
                                                            scale,
                                                            rotation,
                                                            transform))
    {
        component->setPos(pos);
        component->setScale(scale);
        component->setRotation(rotation);
        component->translate(-component->_posOffsetLocal, Transform::eTransform::LOCAL);
        component->translate(-component->_posOffsetWorld, Transform::eTransform::WORLD);
        component->getTransform();
    }

    return (false);
}

bool    ComponentFactory<sTransformComponent>::updateTransforms(glm::vec3& pos, glm::vec3& scale, glm::vec3& rotation, glm::mat4& transform)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    Camera* camera = Renderer::getInstance()->getCurrentCamera();
    ImGuizmo::MODE mode = ImGuizmo::LOCAL;

    if (!camera)
        return (false);

    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    bool changed = false;
    bool cameraRotating = keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_MAINTAINED ||
                            keyboard.getStateMap()[Keyboard::eKey::LEFT_ALT] == Keyboard::eKeyState::KEY_PRESSED;

    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE) || keyboard.isPressed(Keyboard::eKey::T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE) || keyboard.isPressed(Keyboard::eKey::R))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE) || keyboard.isPressed(Keyboard::eKey::E))
        mCurrentGizmoOperation = ImGuizmo::SCALE;

    changed |= ImGui::InputFloat3("Translate", glm::value_ptr(pos), 3);
    changed |= ImGui::InputFloat3("Rotation", glm::value_ptr(rotation), 3);
    changed |= ImGui::InputFloat3("Scale", glm::value_ptr(scale), 3);

    ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));

    ImGuizmo::Enable(enableGuizmos && !cameraRotating);
    ImGuizmo::Manipulate(glm::value_ptr(camera->getView()), glm::value_ptr(camera->getProj()), mCurrentGizmoOperation, mode, glm::value_ptr(transform), nullptr, nullptr);

    if (ImGuizmo::IsOver() && enableGuizmos && !cameraRotating)
    {
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale));
    }

    return (true);
}
