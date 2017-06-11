/**
* @Author   Guillaume Labey
*/

#include <glm/gtc/type_ptr.hpp>

#include <Engine/Core/Components/TransformComponentFactory.hpp>
#include <Engine/EntityFactory.hpp>
#include <Engine/Debug/Logger.hpp>

#include <Engine/Core/Components/RenderComponentFactory.hpp>

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sRenderComponent* component = new sRenderComponent();

    // Initialize some values
    component->animated = json.getBool("animated", false);
    component->modelFile = json.getString("model", "resources/models/default.DAE");
    component->color = json.getColor4f("color", { 1.0f, 1.0f, 1.0f, 1.0f });
    component->ignoreRaycast = json.getBool("ignore_raycast", false);
    component->dynamic = json.getBool("dynamic", false);
    component->hideDynamic = json.getBool("hide_dynamic", false);
    component->display = json.getBool("display", true);

    component->type = EnumManager<Geometry::eType>::stringToEnum(json.getString("type", "MESH"));

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

    ComponentFactory<sRenderComponent>::loadModelMaterialsFromJson(entityType, component->getModelInstance(), json);

    return (component);
}

void    ComponentFactory<sRenderComponent>::loadModelMaterialsFromJson(const std::string& entityType, ModelInstance* modelInstance, const JsonValue& json)
{
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

        modelInstance->getMeshsInstances()[i++]->setMaterial(material);
    }
}

void    ComponentFactory<sRenderComponent>::saveModelMaterialsFromJson(ModelInstance* modelInstance, JsonValue& json)
{
    std::vector<std::string> materialsNames;

    auto& meshsInstances = modelInstance->getMeshsInstances();
    for (auto& meshInstance: meshsInstances)
    {
        Material* material = meshInstance->getMaterial();
        ASSERT(material != nullptr, "A mesh instance should have a material");
        materialsNames.push_back(material->getId());
    }

    json.setStringVec("materials", materialsNames);
}

void    ComponentFactory<sRenderComponent>::updateMaterialsEditor(ModelInstance* modelInstance)
{
    ImGui::Text("\n");
    ImGui::Text("Materials");

    auto& meshsInstances = modelInstance->getMeshsInstances();
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
    json.setBool("dynamic", component->dynamic);
    json.setBool("hide_dynamic", component->hideDynamic);
    json.setBool("display", component->display);

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

    ComponentFactory<sRenderComponent>::saveModelMaterialsFromJson(const_cast<sRenderComponent*>(component)->getModelInstance(), json);

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

    changed |= ImGui::ColorEdit4("color", glm::value_ptr(component->color));
    changed |= ImGui::Checkbox("Ignore mouse raycast", &component->ignoreRaycast);
    changed |= ImGui::Checkbox("Dynamic", &component->dynamic);
    changed |= ImGui::Checkbox("Hide dynamic", &component->hideDynamic);
    changed |= ImGui::Checkbox("Display", &component->display);

    if (Helper::updateComboEnum<Geometry::eType>("Model type", component->type))
    {
        component->_modelInstance = nullptr;
    }

    if (component->type == Geometry::eType::MESH)
    {
        std::string modelName = component->getModel()->getId();
        if (Helper::updateComboString("model", ResourceManager::getInstance()->getResourcesNames<Model>(), modelName))
        {
            component->_modelInstance = nullptr;
            auto model = ResourceManager::getInstance()->getOrLoadResource<Model>(modelName);
            component->modelFile = model->getPath();
        }
    }

    ComponentFactory<sRenderComponent>::updateMaterialsEditor(component->getModelInstance());
    updateAnimationsEditor(component, entity);

    return (changed);
}

bool    ComponentFactory<sRenderComponent>::updateAnimationsEditor(sRenderComponent* component, Entity* entity)
{
    ImGui::Text("\n");
    ImGui::Text("Animations");
    ImGui::SameLine();

    if (component->_animator.isPlaying())
        ComponentFactory<sTransformComponent>::enableGuizmos = false;
    else
        ComponentFactory<sTransformComponent>::enableGuizmos = true;

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
        transform->needUpdate();
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

    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        component->_animator.stop(_lastAnimation->getName());

        sTransformComponent* transform = entity->getComponent<sTransformComponent>();
        transform->needUpdate();
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
                transform->needUpdate();
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
                transform->needUpdate();
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
