/**
* @Author   Guillaume Labey
*/

#include <algorithm>
#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/LevelLoader.hpp>
#include <Engine/Utils/ResourceManager.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Core/ScriptFactory.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/ComponentFactory.hpp>

#include <vector>

std::unordered_map<std::string, IComponentFactory*>  IComponentFactory::_componentsTypes = { COMPONENTS_TYPES(GENERATE_PAIRS) };
std::unordered_map<uint32_t, std::string>  IComponentFactory::_componentsTypesHashs = { COMPONENTS_TYPES(GENERATE_PAIRS_HASHS) };


/*
** IComponentFactory
*/

bool    IComponentFactory::componentTypeExists(const std::string& type)
{
    for (auto &&componentType: _componentsTypes)
    {
        if (componentType.first == type)
            return (true);
    }

    return (false);
}

void    IComponentFactory::initComponent(const std::string& entityType, const std::string& name, const JsonValue& value)
{
    try
    {
        sComponent* component;
        component = _componentsTypes[name]->loadFromJson(entityType, JsonValue(value));
        _componentsTypes[name]->addComponent(entityType, component);
        _componentsTypes[name]->saveComponentJson(entityType, value.get());
    }
    catch(const std::exception& e)
    {
        std::stringstream msg;

        msg << "Failed to load " << name << ": " << e.what();
        EXCEPT(InternalErrorException, msg.str().c_str());
    }
}

sComponent*  IComponentFactory::createComponent(const std::string& entityType, const std::string& name)
{
    return _componentsTypes[name]->clone(entityType);
}

IComponentFactory*   IComponentFactory::getFactory(const std::string& name)
{
    if (!IComponentFactory::componentTypeExists(name))
        EXCEPT(InvalidParametersException, "Cannot get component factory archetype \"%s\": does not exist", name);

    return _componentsTypes[name];
}

std::string IComponentFactory::getComponentNameWithHash(uint32_t hash)
{
    return (_componentsTypesHashs[hash]);
}

std::size_t IComponentFactory::getComponentHashWithName(const std::string& name)
{
    for (auto component: _componentsTypesHashs)
    {
        if (component.second == name)
            return (component.first);
    }
    return (-1);
}

const std::unordered_map<uint32_t, std::string>& IComponentFactory::getComponentsTypesHashs()
{
    return (_componentsTypesHashs);
}

/*
** sRenderComponent
*/

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sRenderComponent* component = new sRenderComponent();

    // Initialize some values
    component->animated = json.getBool("animated", false);
    component->modelFile = json.getString("model", "resources/models/default.DAE");
    component->color = json.getColor4f("color", { 1.0f, 1.0f, 1.0f, 1.0f });
    component->ignoreRaycast = json.getBool("ignore_raycast", false);

    std::string geometryName = json.getString("type", "MESH");
    component->type = EnumManager<Geometry::eType>::stringToEnum(geometryName);

    // Load animations
    {
        auto animations = json.get()["animations"];
        if (animations.size() > 0 && animations.type() != Json::ValueType::arrayValue)
        {
            LOG_ERROR("%s::sRenderComponent loadFromJson error: animations is not an array", entityType.c_str());
            return (component);
        }

        for (const auto& animation: animations)
        {
            // Load animation
            JsonValue animationJon(animation);
            AnimationPtr compAnimation = std::make_shared<Animation>(animationJon.getString("name", "animation"), animationJon.getString("layer", "DEFAULT"));

            // Load animation params
            auto animationParams = animation["params"];
            if (animationParams.type() != Json::ValueType::arrayValue)
            {
                LOG_ERROR("%s::sRenderComponent loadFromJson error: animation params is not an array for animation \"%s\"", entityType.c_str(), compAnimation->getName().c_str());
                component->_animator.addAnimation(compAnimation);
                break;
            }

            for (const auto& animationParam: animationParams)
            {
                JsonValue animationParamJson(animationParam);
                std::string name = animationParamJson.getString("name", "param");
                if (name == "color")
                {
                    std::shared_ptr<ParamAnimation<glm::vec4> > colorParam = std::make_shared<ParamAnimation<glm::vec4> >(name, nullptr, IParamAnimation::eInterpolationType::ABSOLUTE);
                    loadColorParamAnimation(colorParam, animationParamJson);
                    compAnimation->addParamAnimation(colorParam);
                }
                else
                {
                    std::shared_ptr<ParamAnimation<glm::vec3> > translateParam = std::make_shared<ParamAnimation<glm::vec3> >(name, nullptr);
                    loadTranslateParamAnimation(translateParam, animationParamJson);
                    compAnimation->addParamAnimation(translateParam);
                }
            }

            component->_animator.addAnimation(compAnimation);
        }
    }

    // Load materials
    {
        // Init model instance
        ModelInstance* modelInstance = component->getModelInstance();

        std::vector<std::string> materials = json.getStringVec("materials", {});

        uint32_t meshsNb = (uint32_t)modelInstance->getModel()->getMeshs().size();
        uint32_t i = 0;
        for (const std::string& materialName: materials)
        {
            if (i >= meshsNb)
            {
                break;
            }

            Material* material = ResourceManager::getInstance()->getResource<Material>(materialName);
            if (!material)
            {
                EXCEPT(InternalErrorException, "%s::sRenderComponent loadFromJson error: can't find material ", entityType.c_str(), materialName.c_str());
            }

            modelInstance->getMeshsInstances()[i++]->setMaterial(material);
        }
    }

    return (component);
}

void    ComponentFactory<sRenderComponent>::loadTranslateParamAnimation(std::shared_ptr<ParamAnimation<glm::vec3>> paramAnimation, JsonValue& json)
{
    auto keyFrames = json.get()["frames"];
    if (keyFrames.type() != Json::ValueType::arrayValue)
    {
        LOG_ERROR("sRenderComponent loadFromJson error: animation param frames is not an array for animation param \"%s\"", paramAnimation->getName().c_str());
        return;
    }

    for (const auto& keyFrame: keyFrames)
    {
        JsonValue keyFrameJson(keyFrame);
        ParamAnimation<glm::vec3>::sKeyFrame key;
        std::string easingType = keyFrameJson.getString("easing", "NONE");

        key.time = keyFrameJson.getFloat("time", 1.0f);
        key.value = keyFrameJson.getVec3f("value", glm::vec3(0.0f, 0.0f, 0.0f));
        key.easing = EnumManager<IParamAnimation::eEasing>::stringToEnum(easingType);
        paramAnimation->addKeyFrame(key);
    }
}

void    ComponentFactory<sRenderComponent>::loadColorParamAnimation(std::shared_ptr<ParamAnimation<glm::vec4>> paramAnimation, JsonValue& json)
{
    auto keyFrames = json.get()["frames"];
    if (keyFrames.type() != Json::ValueType::arrayValue)
    {
        LOG_ERROR("%s::sRenderComponent loadFromJson error: animation param frames is not an array for animation param \"%s\"", paramAnimation->getName().c_str());
        return;
    }

    for (const auto& keyFrame: keyFrames)
    {
        JsonValue keyFrameJson(keyFrame);
        ParamAnimation<glm::vec4>::sKeyFrame key;
        std::string easingType = keyFrameJson.getString("easing", "NONE");

        key.time = keyFrameJson.getFloat("time", 1.0f);
        key.value = keyFrameJson.getVec4f("value", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        key.easing = EnumManager<IParamAnimation::eEasing>::stringToEnum(easingType);

        paramAnimation->addKeyFrame(key);
    }
}

JsonValue&    ComponentFactory<sRenderComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sRenderComponent* component = static_cast<const sRenderComponent*>(savedComponent ? savedComponent : _components[entityType]);


    json.setBool("animated", component->animated);
    json.setString("model", component->modelFile);
    json.setColor4f("color", component->color);
    json.setString("type", EnumManager<Geometry::eType>::enumToString(component->type));
    json.setBool("ignore_raycast", component->ignoreRaycast);

    // Save animations
    {
        std::vector<JsonValue> animations;
        for (const auto& animation: component->_animator.getAnimations())
        {
            JsonValue animationJson;
            std::vector<JsonValue> paramsAnimation;

            animationJson.setString("name", animation->getName());
            animationJson.setString("layer", animation->getLayer());
            // Save animation params
            for (const auto& paramAnimation: animation->getParamsAnimations())
            {
                JsonValue paramAnimationJson;
                if (paramAnimation->getName() == "color")
                    saveColorParamAnimation(paramAnimation, paramAnimationJson);
                else
                    saveTranslateParamAnimation(paramAnimation, paramAnimationJson);

                paramAnimationJson.setString("name", paramAnimation->getName());
                paramsAnimation.push_back(paramAnimationJson);
            }

            animationJson.setValueVec("params", paramsAnimation);
            animations.push_back(animationJson);
        }

        json.setValueVec("animations", animations);
    }

    // Save materials
    {
        std::vector<std::string> materialsNames;

        auto& meshsInstances = const_cast<sRenderComponent*>(component)->getModelInstance()->getMeshsInstances();
        for (auto& meshInstance: meshsInstances)
        {
            Material* material = meshInstance->getMaterial();
            ASSERT(material != nullptr, "A mesh instance should have a material");
            materialsNames.push_back(material->getId());
        }

        json.setStringVec("materials", materialsNames);
    }

    return (json);
}

void    ComponentFactory<sRenderComponent>::saveTranslateParamAnimation(std::shared_ptr<IParamAnimation> paramAnimation_, JsonValue& json)
{
    auto paramAnimation = std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation_);
    uint32_t totalFrames = (uint32_t)paramAnimation->getKeyFrames().size();
    std::vector<JsonValue> keyFrames;

    for (uint32_t i = 0; i < totalFrames; ++i)
    {
        ParamAnimation<glm::vec3>::sKeyFrame& keyFrame = paramAnimation->getKeyFrames()[i];
        JsonValue keyFrameJson;

        keyFrameJson.setVec3f("value", keyFrame.value);
        keyFrameJson.setFloat("time", keyFrame.time);
        keyFrameJson.setString("easing", EnumManager<IParamAnimation::eEasing>::enumToString(keyFrame.easing));
        keyFrames.push_back(keyFrameJson);
    }

    json.setValueVec("frames", keyFrames);
}

void    ComponentFactory<sRenderComponent>::saveColorParamAnimation(std::shared_ptr<IParamAnimation> paramAnimation_, JsonValue& json)
{
    auto paramAnimation = std::static_pointer_cast<ParamAnimation<glm::vec4>>(paramAnimation_);
    uint32_t totalFrames = (uint32_t)paramAnimation->getKeyFrames().size();
    std::vector<JsonValue> keyFrames;

    for (uint32_t i = 0; i < totalFrames; ++i)
    {
        ParamAnimation<glm::vec4>::sKeyFrame& keyFrame = paramAnimation->getKeyFrames()[i];
        JsonValue keyFrameJson;

        keyFrameJson.setVec4f("value", keyFrame.value);
        keyFrameJson.setFloat("time", keyFrame.time);
        keyFrameJson.setString("easing", EnumManager<IParamAnimation::eEasing>::enumToString(keyFrame.easing));
        keyFrames.push_back(keyFrameJson);
    }

    json.setValueVec("frames", keyFrames);
}

bool    ComponentFactory<sRenderComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sRenderComponent* component = static_cast<sRenderComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;
    bool typeChanged = false;
    bool modelChanged = false;

    changed |= ImGui::ColorEdit4("color", glm::value_ptr(component->color));
    changed |= ImGui::Checkbox("Ignore mouse raycast", &component->ignoreRaycast);
    typeChanged |= Helper::updateComboEnum<Geometry::eType>("Model type", component->type);

    if (component->type == Geometry::eType::MESH)
    {
        static ResourceManager* resourceManager = ResourceManager::getInstance();
        static std::vector<const char*>& modelsString = const_cast<std::vector<const char*>&>(resourceManager->getResourcesNames<Model>());
        auto model = resourceManager->getOrLoadResource<Model>(component->modelFile);
        int selectedModel = static_cast<int>(std::find(modelsString.cbegin(), modelsString.cend(), model->getId()) - modelsString.begin());
        const char** modelsList = modelsString.data();

        if (ImGui::ListBox("model", &selectedModel, modelsList, (int)resourceManager->getResourcesNames<Model>().size(), 4))
        {
            modelChanged = true;
            model = resourceManager->getOrLoadResource<Model>(modelsString[selectedModel]);
            component->modelFile = model->getPath();
        }
    }

    changed |= typeChanged || modelChanged;

    if (typeChanged || modelChanged)
    {
        // Remove the model to auto reload the new model
        component->_modelInstance = nullptr;
    }

    updateMaterialsEditor(component, entity);
    updateAnimationsEditor(component, entity);

    return (changed);
}

bool    ComponentFactory<sRenderComponent>::updateMaterialsEditor(sRenderComponent* component, Entity* entity)
{
    ImGui::Text("\n");
    ImGui::Text("Materials");

    auto& meshsInstances = component->getModelInstance()->getMeshsInstances();
    uint32_t i = 0;
    for (auto& meshInstance: meshsInstances)
    {
        ImGui::PushID(i++);
        Material* material = meshInstance->getMaterial();
        ASSERT(material != nullptr, "A mesh instance should have a material");

        std::string materialName = material->getId();
        if (Helper::updateComboString("mat", ResourceManager::getInstance()->getResourcesNames<Material>(), materialName))
        {
            meshInstance->setMaterial(ResourceManager::getInstance()->getResource<Material>(materialName));
        }
        ImGui::PopID();
    }
    return (false);
}

bool    ComponentFactory<sRenderComponent>::updateAnimationsEditor(sRenderComponent* component, Entity* entity)
{
    ImGui::Text("\n");
    ImGui::Text("Animations");
    ImGui::SameLine();

    // Add new animation button
    if (ImGui::Button("Create"))
    {
        AnimationPtr animation = std::make_shared<Animation>("animation");
        component->_animator.addAnimation(animation);
        component->_animator.play("animation");
    }

    // Animations list
    ImGui::BeginChild("Animations", ImVec2(0, 100), true);
    for (auto animation: component->_animator.getAnimations())
    {
        if (ImGui::Selectable(animation->getName().c_str(), _lastAnimation == animation))
        {
            component->_animator.play(animation->getName(), animation->isLoop());
            _lastAnimation = animation;
        }
    }
    ImGui::EndChild();

    if (!_lastAnimation)
        return (false);

    // Animation params style
    ImGui::PushStyleColor(ImGuiCol_Header, ImColor(0.27f, 0.51f, 0.70f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImColor(0.39f, 0.58f, 0.92f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImColor(0.49f, 0.68f, 0.92f, 1.0f));

    // Edit animation name
    {
        ImGui::Text("\n");
        const std::string& animationName = _lastAnimation->getName();
        std::vector<char> animationNameVec(animationName.cbegin(), animationName.cend());
        animationNameVec.push_back(0);
        animationNameVec.resize(64);

        if (ImGui::InputText("Name", animationNameVec.data(), animationNameVec.size()))
        {
            _lastAnimation->setName(animationNameVec.data());
        }
    }

    // Edit animation layer
    {
        ImGui::Text("\n");
        const std::string& animationLayer = _lastAnimation->getLayer();
        std::vector<char> animationLayerVec(animationLayer.cbegin(), animationLayer.cend());
        animationLayerVec.push_back(0);
        animationLayerVec.resize(64);

        if (ImGui::InputText("Layer", animationLayerVec.data(), animationLayerVec.size()))
        {
            component->_animator.stop(_lastAnimation->getName());
            _lastAnimation->setLayer(animationLayerVec.data());
        }
    }
    ImGui::Text("\n");

    // Delete animation
    if (ImGui::Button("Delete"))
    {
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();
        component->_animator.removeAnimation(_lastAnimation);
        transform->needUpdate = true;
        ImGui::PopStyleColor(3);
        return (false);
    }

    // Add animation param
    ImGui::SameLine();
    if (ImGui::Button("New param"))
    {
        ImGui::OpenPopup("params");
    }

    ImGui::SameLine();
    if (ImGui::Button("Play"))
    {
        component->_animator.play(_lastAnimation->getName(), _lastAnimation->isLoop());
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset"))
    {
        _lastAnimation->reset();
        _lastAnimation->update(0);
    }

    bool loop = _lastAnimation->isLoop();
    if (ImGui::Checkbox("Loop (preview, not saved)", &loop))
    {
        component->_animator.play(_lastAnimation->getName(), loop);
    }

    // New animation param selection
    ImGui::Text("\n");
    ImGui::Text("Params:");
    if (_lastAnimation->getParamsAnimations().size() == 0)
    {
        ImGui::Text("The animation has no param yet");
    }
    if (ImGui::BeginPopup("params"))
    {
        static const std::vector<std::string> params = { "color", "position", "scale", "rotation" };
        for (auto param: params)
        {
            // Add new param
            if (ImGui::Button(param.c_str()))
            {
                if (param == "color")
                    _lastAnimation->addParamAnimation(std::make_shared<ParamAnimation<glm::vec4> >(param, nullptr, IParamAnimation::eInterpolationType::ABSOLUTE));
                else
                    _lastAnimation->addParamAnimation(std::make_shared<ParamAnimation<glm::vec3> >(param, nullptr));
                EntityFactory::initAnimations(entity);
            }
        }
        ImGui::EndPopup();
    }

    // Edit animation params
    updateParamsAnimationsEditor(_lastAnimation, entity);
    ImGui::PopStyleColor(3);

    return (false);
}

bool    ComponentFactory<sRenderComponent>::updateParamsAnimationsEditor(AnimationPtr playedAnimation, Entity* entity)
{
    uint32_t frameNb = 0;

    auto& paramsAnimations = playedAnimation->getParamsAnimations();
    for (uint32_t i = 0; i < paramsAnimations.size();)
    {
        auto paramAnimation = paramsAnimations[i];
        // Animation param display
        if (ImGui::CollapsingHeader(paramAnimation->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushID(paramAnimation->getName().c_str());
            // Remove animation param
            if (ImGui::Button("Remove"))
            {
                sTransformComponent* transform = entity->getComponent<sTransformComponent>();
                playedAnimation->removeParamAnimation(paramAnimation);
                transform->needUpdate = true;
                ImGui::PopID();
                continue;
            }
            else
                ++i;

            if (paramAnimation->getName() == "color")
                updateAnimationParamColor(paramAnimation, frameNb);
            else
                updateAnimationParamTranslate(entity, paramAnimation, frameNb);

            ImGui::PopID();
        }
        else
            ++i;
    }
    ImGui::Text("\n");
    return (false);
}

bool    ComponentFactory<sRenderComponent>::updateAnimationParamTranslate(Entity* entity, std::shared_ptr<IParamAnimation> paramAnimation_, uint32_t& frameNb)
{
    auto paramAnimation = std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation_);

    // add animation param key frame
    ImGui::SameLine();
    if (ImGui::Button("Add key frame"))
    {
        ParamAnimation<glm::vec3>::sKeyFrame keyFrame;
        keyFrame.time = 1.0f;
        keyFrame.value = glm::vec3(0.0f, 0.0f, 0.0f);
        keyFrame.easing = IParamAnimation::eEasing::NONE;
        paramAnimation->addKeyFrame(keyFrame);
    }

    uint32_t totalFrames = (uint32_t)paramAnimation->getKeyFrames().size();
    bool sortKeyFrames = false;

    ImGui::Text("\n");
    for (uint32_t i = 0; i < totalFrames; ++i)
    {
        ParamAnimation<glm::vec3>::sKeyFrame& keyFrame = paramAnimation->getKeyFrame(i);

        ImGui::Text("Frame %d ", keyFrame.id);
        ImGui::PushID(keyFrame.id);
        frameNb++;

        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            paramAnimation->removeKeyFrame(i);
            --i;
            --totalFrames;

            if (totalFrames == 0)
            {
                sTransformComponent* transform = entity->getComponent<sTransformComponent>();
                transform->needUpdate = true;
            }
        }
        else
        {
            ImGui::InputFloat3("value", glm::value_ptr(keyFrame.value));
            if (ImGui::InputFloat("time", &keyFrame.time, 1))
                sortKeyFrames = true;

            // Easing type Combo box
            Helper::updateComboEnum<IParamAnimation::eEasing>("Easing", keyFrame.easing);
        }

        ImGui::PopID();
        ImGui::Text("\n");
    }

    if (sortKeyFrames)
        paramAnimation->sortKeyFrames();

    return (false);
}

bool    ComponentFactory<sRenderComponent>::updateAnimationParamColor(std::shared_ptr<IParamAnimation> paramAnimation_, uint32_t& frameNb)
{
    auto paramAnimation = std::static_pointer_cast<ParamAnimation<glm::vec4>>(paramAnimation_);

    // add animation param key frame
    ImGui::SameLine();
    if (ImGui::Button("Add key frame"))
    {
        ParamAnimation<glm::vec4>::sKeyFrame keyFrame;
        keyFrame.time = 1.0f;
        keyFrame.value = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        keyFrame.easing = IParamAnimation::eEasing::NONE;
        paramAnimation->addKeyFrame(keyFrame);
    }

    uint32_t totalFrames = (uint32_t)paramAnimation->getKeyFrames().size();
    bool sortKeyFrames = false;

    ImGui::Text("\n");
    for (uint32_t i = 0; i < totalFrames; ++i)
    {
        ParamAnimation<glm::vec4>::sKeyFrame& keyFrame = paramAnimation->getKeyFrame(i);

        ImGui::Text("Frame %d ", keyFrame.id);
        ImGui::PushID(keyFrame.id);
        frameNb++;

        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            paramAnimation->removeKeyFrame(i);
            --i;
            --totalFrames;
        }
        else
        {
            ImGui::ColorEdit4("value", glm::value_ptr(keyFrame.value));
            if (ImGui::InputFloat("time", &keyFrame.time))
                sortKeyFrames = true;

            // Easing type listBox
            Helper::updateComboEnum<IParamAnimation::eEasing>("Easing", keyFrame.easing);
        }

        ImGui::PopID();
        ImGui::Text("\n");
    }

    if (sortKeyFrames)
        paramAnimation->sortKeyFrames();

    return (false);
}

/*
** sRigidBodyComponent
*/

sComponent* ComponentFactory<sRigidBodyComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sRigidBodyComponent* component = new sRigidBodyComponent();

    component->gravity = json.getVec3f("gravity", {0.0f, 0.0f, 0.0f});
    component->velocity = json.getVec3f("velocity", {0.0f, 0.0f, 0.0f});
    component->collisionsEnabled = json.getBool("collisions_enabled", true);

    return (component);
}

JsonValue&    ComponentFactory<sRigidBodyComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sRigidBodyComponent* component = static_cast<const sRigidBodyComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("gravity", component->gravity);
    json.setVec3f("velocity", component->velocity);
    json.setBool("collisions_enabled", component->collisionsEnabled);

    return (json);
}


/*
** sBoxColliderComponent
*/

sComponent* ComponentFactory<sBoxColliderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sBoxColliderComponent* component = new sBoxColliderComponent();

    component->pos = json.getVec3f("pos", { 0.0f, 0.0f, 0.0f });
    component->size = json.getVec3f("size", { 2.0f, 2.0f, 2.0f });
    component->isTrigger = json.getBool("isTrigger", false);

    return (component);
}

JsonValue&    ComponentFactory<sBoxColliderComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sBoxColliderComponent* component = static_cast<const sBoxColliderComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("pos", component->pos);
    json.setVec3f("size", component->size);
    json.setBool("isTrigger", component->isTrigger);

    return (json);
}

bool    ComponentFactory<sBoxColliderComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sBoxColliderComponent* component = static_cast<sBoxColliderComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    if (ImGui::Button("Reset"))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (render)
        {
            component->pos = glm::vec3(render->getModel()->getMin().x - 0.5f, render->getModel()->getMin().y - 0.5f, render->getModel()->getMin().z - 0.5f);
            component->size.x = (render->getModel()->getSize().x + 1.0f) / SIZE_UNIT;
            component->size.y = (render->getModel()->getSize().y + 1.0f) / SIZE_UNIT;
            component->size.z = (render->getModel()->getSize().z + 1.0f) / SIZE_UNIT;
        }
    }
    if (ImGui::Button(component->display ? "Hide" : "Display"))
    {
        component->display = !component->display;
    }

    ImGui::InputFloat3("position", glm::value_ptr(component->pos), 3);
    ImGui::InputFloat3("size", glm::value_ptr(component->size), 3);

    return (changed);
}

/*
** sSphereColliderComponent
*/

sComponent* ComponentFactory<sSphereColliderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sSphereColliderComponent* component = new sSphereColliderComponent();

    component->pos = json.getVec3f("pos", { 0.0f, 0.0f, 0.0f });
    component->radius = json.getFloat("radius", 2.0f);
    component->isTrigger = json.getBool("isTrigger", false);

    return (component);
}

JsonValue&    ComponentFactory<sSphereColliderComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sSphereColliderComponent* component = static_cast<const sSphereColliderComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setVec3f("pos", component->pos);
    json.setFloat("radius", component->radius);
    json.setBool("isTrigger", component->isTrigger);

    return (json);
}

bool    ComponentFactory<sSphereColliderComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sSphereColliderComponent* component = static_cast<sSphereColliderComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    if (ImGui::Button("Reset"))
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        if (render)
        {
            float modelMaxSize = render->getModel()->getSize().x;
            if (render->getModel()->getSize().y > modelMaxSize)
                modelMaxSize = render->getModel()->getSize().y;
            if (render->getModel()->getSize().z > modelMaxSize)
                modelMaxSize = render->getModel()->getSize().z;
            component->radius = (modelMaxSize / 2.0f + 1.0f) / SIZE_UNIT;
            component->pos = glm::vec3(render->getModel()->getMin().x + (render->getModel()->getSize().x / 2.0f),
                                        render->getModel()->getMin().y + (render->getModel()->getSize().y / 2.0f),
                                        render->getModel()->getMin().z + (render->getModel()->getSize().z / 2.0f));
        }

    }
    if (ImGui::Button(component->display ? "Hide" : "Display"))
    {
        component->display = !component->display;
    }


    if (ImGui::Button(component->isTrigger ? "Trigger" : "No trigger"))
    {
        component->isTrigger = !component->isTrigger;
    }

    ImGui::InputFloat3("position", glm::value_ptr(component->pos), 3);
    ImGui::InputFloat("radius", &component->radius, 3.0f);

    return (changed);
}


/*
** sTypeComponent
*/

eEntityType ComponentFactory<sTypeComponent>::stringToEntityType(const std::string& entityTypeStr)
{
    if (entityTypeStr == "CHARACTER")
        return eEntityType::CHARACTER;
    else if (entityTypeStr == "OBJECT")
        return eEntityType::OBJECT;
    else if (entityTypeStr == "TILE_WALKABLE")
        return eEntityType::TILE_WALKABLE;
    else if (entityTypeStr == "TILE_NOT_WALKABLE")
        return eEntityType::TILE_NOT_WALKABLE;
    else if (entityTypeStr == "TILE_STAIRS_UP")
        return eEntityType::TILE_STAIRS_UP;
    else if (entityTypeStr == "TILE_STAIRS_DOWN")
        return eEntityType::TILE_STAIRS_DOWN;

    EXCEPT(NotImplementedException, "Failed to load sTypeComponent:  the entity type %s does not exist", entityTypeStr);
}

std::string ComponentFactory<sTypeComponent>::entityTypeToString(eEntityType entityType)
{
    if (entityType == eEntityType::CHARACTER)
        return ("CHARACTER");
    else if (entityType == eEntityType::OBJECT)
        return ("OBJECT");
    else if (entityType == eEntityType::TILE_WALKABLE)
        return ("TILE_WALKABLE");
    else if (entityType == eEntityType::TILE_NOT_WALKABLE)
        return ("TILE_NOT_WALKABLE");
    else if (entityType == eEntityType::TILE_STAIRS_UP)
        return ("TILE_STAIRS_UP");
    else if (entityType == eEntityType::TILE_STAIRS_DOWN)
        return ("TILE_STAIRS_DOWN");

    EXCEPT(NotImplementedException, "Failed to save sTypeComponent:  the entity type does not exist");
}


JsonValue&    ComponentFactory<sTypeComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sTypeComponent* component = static_cast<const sTypeComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setString("type", entityTypeToString(component->type));

    return (json);
}

sComponent* ComponentFactory<sTypeComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTypeComponent* component = new sTypeComponent();

    component->type = stringToEntityType(json.getString("type", "TILE_WALKABLE"));

    return (component);
}


/*
** sParticleEmitterComponent
*/

sComponent* ComponentFactory<sParticleEmitterComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sParticleEmitterComponent* component = new sParticleEmitterComponent();
    JsonValue color(json.get("color", {}));
    JsonValue size = json.get("size", {});

    component->rate = json.getFloat("rate", 0.0f);
    component->maxParticles = json.getUInt("max_particles", 50);
    component->emitterLife = json.getFloat("emitter_life", 0.0f);
    component->life = json.getUInt("life", 80);
    component->lifeVariance = json.getUInt("life_variance", 0);
    component->angle = json.getFloat("angle", 50.0f);
    component->angleVariance = json.getFloat("angle_variance", 0.0f);
    component->speed = json.getFloat("speed", 30.0f);
    component->speedVariance =  json.getFloat("speed_variance", 0.0f);

    if (color.size() > 0)
    {
        component->colorStart = color.getColor4f("start", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinish = color.getColor4f("finish", { 1.0f, 1.0f, 1.0f, 1.0f });
    }
    else
    {
        component->colorStart = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        component->colorFinish = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    }

    if (size.size() > 0)
    {
        component->sizeStart =  size.getFloat("start", 1.0f);
        component->sizeFinish =  size.getFloat("finish", 1.0f);
        component->sizeStartVariance =  size.getFloat("start_variance", 1.0f);
        component->sizeFinishVariance =  size.getFloat("finish_variance", 1.0f);
    }
    else
    {
        component->sizeStart = 1.0f;
        component->sizeFinish = 0.0f;
        component->sizeStartVariance = 0.0f;
        component->sizeFinishVariance = 0.0f;
    }

    component->texture = json.getString("texture", "");

    return (component);
}

JsonValue&    ComponentFactory<sParticleEmitterComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue color;
    JsonValue size;
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sParticleEmitterComponent* component = static_cast<const sParticleEmitterComponent*>(savedComponent ? savedComponent : _components[entityType]);

    // Write colors
    color.setColor4f("start", component->colorStart);
    color.setColor4f("finish", component->colorFinish);
    json.setValue("color", color);

    // Write size
    size.setFloat("start", component->sizeStart);
    size.setFloat("start_variance", component->sizeStartVariance);
    size.setFloat("finish", component->sizeFinish);
    size.setFloat("finish_variance", component->sizeFinishVariance);
    json.setValue("size", size);

    json.setFloat("rate", component->rate);
    json.setUInt("max_particles", component->maxParticles);
    json.setFloat("emitter_life", component->emitterLife);
    json.setUInt("life", component->life);
    json.setUInt("life_variance", component->lifeVariance);
    json.setFloat("angle", component->angle);
    json.setFloat("angle_variance", component->angleVariance);
    json.setFloat("speed", component->speed);
    json.setFloat("speed_variance", component->speedVariance);

    json.setString("texture", component->texture);

    return (json);
}

bool    ComponentFactory<sParticleEmitterComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    changed |= ImGui::SliderFloat("Emitter life (0 to disable)", &component->emitterLife, 0.0f, 10.0f);
    changed |= ImGui::SliderFloat("Rate", &component->rate, 0.0f, 3.0f);
    changed |= ImGui::SliderInt("Max particles", (int*)&component->maxParticles, 0, 500);
    changed |= ImGui::SliderFloat("Angle", &component->angle, 0.0f, 360.0f);
    changed |= ImGui::SliderFloat("Angle variance", &component->angleVariance, 0.0f, 360.0f);
    changed |= ImGui::SliderFloat("Speed", &component->speed, 0.0f, 200.0f);
    changed |= ImGui::SliderFloat("Speed variance", &component->speedVariance, 0.0f, 200.0f);
    changed |= ImGui::SliderInt("Life", (int*)&component->life, 0, 200);
    changed |= ImGui::SliderInt("Life variance", (int*)&component->lifeVariance, 0, 200);
    changed |= ImGui::ColorEdit4("Start color", glm::value_ptr(component->colorStart));
    changed |= ImGui::ColorEdit4("Finish color", glm::value_ptr(component->colorFinish));
    changed |= ImGui::SliderFloat("Start size", &component->sizeStart, 0.0f, 5.0f);
    changed |= ImGui::SliderFloat("Finish size", &component->sizeFinish, 0.0f, 5.0f);
    changed |= ImGui::SliderFloat("Start size variance", &component->sizeStartVariance, 0.0f, 5.0f);
    changed |= ImGui::SliderFloat("Finish size variance", &component->sizeFinishVariance, 0.0f, 5.0f);

    return (changed);
}

/*
** sNameComponent
*/

sComponent* ComponentFactory<sNameComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sNameComponent*   component;

    component = new sNameComponent();

    component->value = json.getString("name", "default");

    return (component);
}

JsonValue&    ComponentFactory<sNameComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sNameComponent* component = static_cast<const sNameComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setString("name", component->value);

    return (json);
}


/*
** sTransformComponent
*/

sComponent* ComponentFactory<sTransformComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTransformComponent*   component;

    component = new sTransformComponent();

    component->scale = json.getVec3f("scale", { 1.0f, 1.0f, 1.0f });
    component->pos = json.getVec3f("pos", { 0.0f, 0.0f, 0.0f });
    component->rotation = json.getVec3f("rotation", { 0.0f, 0.0f, 0.0f });

    return (component);
}

JsonValue&    ComponentFactory<sTransformComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sTransformComponent* component = static_cast<const sTransformComponent*>(savedComponent ? savedComponent : _components[entityType]);


    json.setVec3f("scale", component->scale);
    json.setVec3f("pos", component->pos);
    json.setVec3f("rotation", component->rotation);

    return (json);
}

bool    ComponentFactory<sTransformComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sTransformComponent* component = static_cast<sTransformComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    glm::vec3 savedRot = component->rotation;
    component->rotation = glm::vec3(0.0f);
    component->updateTransform();

    if (ComponentFactory<sTransformComponent>::updateTransforms(component->pos,
                                                            component->scale,
                                                            component->rotation,
                                                            savedRot,
                                                            component->transform,
                                                            ImGuizmo::WORLD))
    {
        component->rotation += savedRot;
        component->updateTransform();
    }

    return (false);
}

bool    ComponentFactory<sTransformComponent>::updateTransforms(glm::vec3& pos, glm::vec3& scale, glm::vec3& rotation, glm::vec3& savedRotation, glm::mat4& transform, ImGuizmo::MODE mode)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return (false);

    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
    bool changed = false;

    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE) || keyboard.isPressed(Keyboard::eKey::T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE) || keyboard.isPressed(Keyboard::eKey::R))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE) || keyboard.isPressed(Keyboard::eKey::E))
        mCurrentGizmoOperation = ImGuizmo::SCALE;

    changed |= ImGui::InputFloat3("Translate", glm::value_ptr(pos), 3);
    changed |= ImGui::InputFloat3("Rotation", glm::value_ptr(savedRotation), 3);
    changed |= ImGui::InputFloat3("Scale", glm::value_ptr(scale), 3);

    ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));

    ImGuizmo::Manipulate(glm::value_ptr(camera->getView()), glm::value_ptr(camera->getProj()), mCurrentGizmoOperation, mode, glm::value_ptr(transform), nullptr, nullptr);
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale));

    return (true);
}


/*
** sButtonComponent
*/

sComponent* ComponentFactory<sButtonComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sButtonComponent*  component;

    component = new sButtonComponent();

    component->action = EnumManager<sButtonComponent::eAction>::stringToEnum(json.getString("action", "NONE"));
    component->actionLevel = json.getString("action_level", "");

    return component;
}

JsonValue&    ComponentFactory<sButtonComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sButtonComponent* component = static_cast<const sButtonComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setString("action", EnumManager<sButtonComponent::eAction>::enumToString(component->action));
    json.setString("action_level", component->actionLevel);

    return (json);
}

bool    ComponentFactory<sButtonComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sButtonComponent* component = static_cast<sButtonComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    changed |= Helper::updateComboEnum<sButtonComponent::eAction>("Action", component->action);

    if (component->action == sButtonComponent::eAction::ADD_LEVEL ||
        component->action == sButtonComponent::eAction::REPLACE_CURRENT_LEVEL)
    {
        Helper::updateComboString("Level", LevelLoader::getInstance()->getLevels(), component->actionLevel);
    }

    return (changed);
}

/*
** sScriptComponent
*/

sComponent* ComponentFactory<sScriptComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sScriptComponent*  component;

    component = new sScriptComponent();

    component->scriptNames = json.getStringVec("class", {});

    return component;
}

JsonValue&    ComponentFactory<sScriptComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sScriptComponent* component = static_cast<const sScriptComponent*>(savedComponent ? savedComponent : _components[entityType]);

    json.setStringVec("class", component->scriptNames);

    return (json);
}

bool    ComponentFactory<sScriptComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sScriptComponent* component = static_cast<sScriptComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    ImGui::Text("\n");
    ImGui::Text("Scripts");
    ImGui::SameLine();

    // Add new script button
    if (ImGui::Button("New"))
    {
        ImGui::OpenPopup("scripts");
    }

    // Display new scripts that can be added
    if (ImGui::BeginPopup("scripts"))
    {
        for (auto script: ScriptFactory::getScriptsNames())
        {
            // Entity does not have this script
            if (std::find(component->scriptNames.cbegin(), component->scriptNames.cend(), script) == component->scriptNames.cend())
            {
                // Script button pressed, add it to the entity
                if (ImGui::Button(script))
                {
                    component->scriptNames.push_back(script);
                }
            }
        }
        ImGui::EndPopup();
    }

    // Scripts list
    ImGui::BeginChild("Scripts", ImVec2(0, 100), true);
    for (auto& script: component->scriptNames)
    {
        if (ImGui::Selectable(script.c_str(), component->selectedScript == script))
        {
            component->selectedScript = script;
        }
    }
    ImGui::EndChild();

    if (component->selectedScript.size() == 0)
        return (changed);

    ImGui::Text("\n");
    ImGui::Text("%s script options\n", component->selectedScript.c_str());

    // Delete script
    if (ImGui::Button("Delete"))
    {
        auto& eraseIt = std::remove(component->scriptNames.begin(), component->scriptNames.end(), component->selectedScript);
        component->scriptNames.erase(eraseIt);
        return (true);
    }

    return (changed);
}

/*
** sUiComponent
*/

sComponent* ComponentFactory<sUiComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sUiComponent*  component;

    component = new sUiComponent();

    component->offset = json.getVec2f("offset", { 0.0f, 0.0f });
    component->horizontalAlignment = EnumManager<eHorizontalAlignment>::stringToEnum(json.getString("horizontal_alignment", "MIDDLE"));
    component->verticalAlignment = EnumManager<eVerticalAlignment>::stringToEnum(json.getString("vertical_alignment", "MIDDLE"));

    return component;
}

JsonValue&    ComponentFactory<sUiComponent>::saveToJson(const std::string& entityType, const sComponent* savedComponent, JsonValue* toJson)
{
    JsonValue& json = toJson ? *toJson : _componentsJson[entityType];
    const sUiComponent* component = static_cast<const sUiComponent*>(savedComponent ? savedComponent : _components[entityType]);


    json.setVec2f("offset", component->offset);
    json.setString("horizontal_alignment", EnumManager<eHorizontalAlignment>::enumToString(component->horizontalAlignment));
    json.setString("vertical_alignment", EnumManager<eVerticalAlignment>::enumToString(component->verticalAlignment));

    return (json);
}

bool    ComponentFactory<sUiComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sUiComponent* component = static_cast<sUiComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;

    changed |= Helper::updateComboEnum<eHorizontalAlignment>("Horizontal alignment", component->horizontalAlignment);
    changed |= Helper::updateComboEnum<eVerticalAlignment>("Vertical alignment", component->verticalAlignment);
    changed |= ImGui::InputFloat("horizontal offset", &component->offset.x, 1.0f, ImGuiInputTextFlags_AllowTabInput);
    changed |= ImGui::InputFloat("vertical offset", &component->offset.y, 1.0f, ImGuiInputTextFlags_AllowTabInput);

    if (changed)
    {
        component->needUpdate = true;
    }

    return (changed);
}
