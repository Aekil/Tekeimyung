#include <algorithm>
#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Utils/RessourceManager.hpp>
#include <Engine/Graphics/Camera.hpp>

#include <Game/ComponentFactory.hpp>

std::unordered_map<std::string, IComponentFactory*>  IComponentFactory::_componentsTypes = { COMPONENTS_TYPES(GENERATE_PAIRS) };
std::unordered_map<std::size_t, std::string>  IComponentFactory::_componentsTypesHashs = { COMPONENTS_TYPES(GENERATE_PAIRS_HASHS) };


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

std::string IComponentFactory::getComponentNameWithHash(std::size_t hash)
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

const std::unordered_map<std::size_t, std::string>& IComponentFactory::getComponentsTypesHashs()
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
    component->texture = json.getString("texture", "");

    std::string geometryName = json.getString("type", "MESH");
    component->type = Geometry::getGeometryType(geometryName);

    return (component);
}

JsonValue&    ComponentFactory<sRenderComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sRenderComponent* component = static_cast<sRenderComponent*>(_components[entityType]);


    json.setBool("animated", component->animated);
    json.setString("model", component->modelFile);
    json.setColor4f("color", component->color);
    json.setString("type", Geometry::getGeometryTypeString(component->type));
    json.setString("texture", component->texture);

    return (json);
}

bool    ComponentFactory<sRenderComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sRenderComponent* component = static_cast<sRenderComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    bool changed = false;
    bool typeChanged = false;
    bool textureChanged = false;
    bool modelChanged = false;

    changed |= ImGui::ColorEdit3("color", glm::value_ptr(component->color));

    static std::vector<const char*>& typesString = const_cast<std::vector<const char*>&>(Geometry::getTypesString());
    int selectedType = static_cast<int>(std::find(typesString.cbegin(), typesString.cend(), Geometry::getGeometryTypeString(component->type)) - typesString.begin());
    const char** typesList = typesString.data();

    typeChanged = ImGui::ListBox("Model type", &selectedType, typesList, (int)Geometry::getTypesString().size(), 4);
    if (typeChanged)
        component->type = Geometry::getGeometryType(typesString[selectedType]);

    // Plan
    if (component->type == Geometry::eType::PLANE)
    {
        // Plan texture
        {
            static RessourceManager* resourceManager = RessourceManager::getInstance();
            static std::vector<const char*>& texturesString = const_cast<std::vector<const char*>&>(resourceManager->getTexturesNames());
            const char** texturesList = texturesString.data();
            int selectedTexture = -1;
            Texture* texture;

            if (component->texture.size() > 0)
            {
                texture = &resourceManager->getTexture(component->texture);
                selectedTexture = static_cast<int>(std::find(texturesString.cbegin(), texturesString.cend(), texture->getId()) - texturesString.begin());
            }

            if (ImGui::ListBox("texture", &selectedTexture, texturesList, (int)resourceManager->getTexturesNames().size(), 4))
            {
                textureChanged = true;
                texture = &resourceManager->getTexture(texturesString[selectedTexture]);
                component->texture = texture->getPath();
            }
        }

    }
    // MESH
    else if (component->type == Geometry::eType::MESH)
    {
        static RessourceManager* resourceManager = RessourceManager::getInstance();
        static std::vector<const char*>& modelsString = const_cast<std::vector<const char*>&>(resourceManager->getModelsNames());
        auto model = resourceManager->getModel(component->modelFile);
        int selectedModel = static_cast<int>(std::find(modelsString.cbegin(), modelsString.cend(), model->getId()) - modelsString.begin());
        const char** modelsList = modelsString.data();

        if (ImGui::ListBox("model", &selectedModel, modelsList, (int)resourceManager->getModelsNames().size(), 4))
        {
            modelChanged = true;
            model = resourceManager->getModel(modelsString[selectedModel]);
            component->modelFile = model->getPath();
        }
    }

    changed |= typeChanged || textureChanged || modelChanged;

    if (typeChanged || textureChanged || modelChanged)
    {
        // Remove the model to auto reload the new model
        component->_model = nullptr;
    }

    return (changed);
}


/*
** sPositionComponent
*/

sComponent* ComponentFactory<sPositionComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sPositionComponent* component = new sPositionComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);
    component->z = json.getFloat("z", 0.0f);

    return (component);
}

JsonValue&    ComponentFactory<sPositionComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sPositionComponent* component = static_cast<sPositionComponent*>(_components[entityType]);

    json.setFloat("x", component->value.x);
    json.setFloat("y", component->value.y);
    json.setFloat("z", component->z);

    return (json);
}


/*
** sInputComponent
*/

sComponent* ComponentFactory<sInputComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    Keyboard keyboard;
    auto &&keyMap = keyboard.getStringMap();
    sInputComponent* component = new sInputComponent();

    component->moveLeft = keyMap[json.getString("left", "Q")];
    component->moveRight = keyMap[json.getString("right", "D")];
    component->moveUp = keyMap[json.getString("up", "Z")];
    component->moveDown = keyMap[json.getString("down", "S")];

    return (component);
}

JsonValue&    ComponentFactory<sInputComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sInputComponent* component = static_cast<sInputComponent*>(_components[entityType]);
    Keyboard keyboard;

    json.setString("left", keyboard.keyToString(component->moveLeft));
    json.setString("right", keyboard.keyToString(component->moveRight));
    json.setString("up", keyboard.keyToString(component->moveUp));
    json.setString("down", keyboard.keyToString(component->moveDown));

    return (json);
}


/*
** sDirectionComponent
*/

sComponent* ComponentFactory<sDirectionComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sDirectionComponent* component = new sDirectionComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);
    component->speed =  json.getFloat("speed", 1.0f);

    return (component);
}

JsonValue&    ComponentFactory<sDirectionComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sDirectionComponent* component = static_cast<sDirectionComponent*>(_components[entityType]);

    json.setFloat("x", component->value.x);
    json.setFloat("y", component->value.y);
    json.setFloat("speed", component->speed);

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

JsonValue&    ComponentFactory<sBoxColliderComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sBoxColliderComponent* component = static_cast<sBoxColliderComponent*>(_components[entityType]);

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
        component->pos = glm::vec3(render->_model->getMin().x - 0.5f, render->_model->getMin().y - 0.5f, render->_model->getMin().z - 0.5f);
        component->size.x = (render->_model->getSize().x + 1.0f) / SIZE_UNIT;
        component->size.y = (render->_model->getSize().y + 1.0f) / SIZE_UNIT;
        component->size.z = (render->_model->getSize().z + 1.0f) / SIZE_UNIT;
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

JsonValue&    ComponentFactory<sSphereColliderComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sSphereColliderComponent* component = static_cast<sSphereColliderComponent*>(_components[entityType]);

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

        float modelMaxSize = render->_model->getSize().x;
        if (render->_model->getSize().y > modelMaxSize)
            modelMaxSize = render->_model->getSize().y;
        if (render->_model->getSize().z > modelMaxSize)
            modelMaxSize = render->_model->getSize().z;
        component->radius = (modelMaxSize / 2.0f + 1.0f) / SIZE_UNIT;
        component->pos = glm::vec3(render->_model->getMin().x + (render->_model->getSize().x / 2.0f),
                                    render->_model->getMin().y + (render->_model->getSize().y / 2.0f),
                                    render->_model->getMin().z + (render->_model->getSize().z / 2.0f));
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
** sGravityComponent
*/

sComponent* ComponentFactory<sGravityComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sGravityComponent* component = new sGravityComponent();

    component->value.x = json.getFloat("x", 0.0f);
    component->value.y = json.getFloat("y", 0.0f);

    return (component);
}

JsonValue&    ComponentFactory<sGravityComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sGravityComponent* component = static_cast<sGravityComponent*>(_components[entityType]);

    json.setFloat("x", component->value.x);
    json.setFloat("y", component->value.y);

    return (json);
}

/*
** sResolutionComponent
*/
sComponent* ComponentFactory<sResolutionComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sResolutionComponent* component = new sResolutionComponent();

    component->collidingState= eCollisionState::NO_COLLISION;
    component->entityId = -1;
    component->onCollisionEnter = Resolutions::getInstance()->getResolutionByName(json.getString("onCollisionEnter", ""));

    return (component);
}

JsonValue&    ComponentFactory<sResolutionComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sResolutionComponent* component = static_cast<sResolutionComponent*>(_components[entityType]);

    json.setString("onCollisionEnter", "onCollisionEnterPlayer");

    return (json);
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


JsonValue&    ComponentFactory<sTypeComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sTypeComponent* component = static_cast<sTypeComponent*>(_components[entityType]);

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
** sAIComponent
*/

sComponent* ComponentFactory<sAIComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sAIComponent* component = new sAIComponent();

    return (component);
}


/*
** sPlayerComponent
*/

sComponent* ComponentFactory<sPlayerComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sPlayerComponent* component = new sPlayerComponent();

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
    component->spawnNb = json.getUInt("spawn_nb", 1);
    component->emitterLife = json.getFloat("emitter_life", 0.0f);
    component->life = json.getUInt("life", 80);
    component->lifeVariance = json.getUInt("life_variance", 0);
    component->angle = json.getFloat("angle", 30.0f);
    component->angleVariance = json.getFloat("angle_variance", 0.0f);
    component->speed = json.getFloat("speed", 30.0f);
    component->speedVariance =  json.getFloat("speed_variance", 0.0f);

    if (color.size() > 0)
    {
        component->colorStart = color.getColor4f("start", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorStartVariance = color.getColor4f("start_variance", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinish = color.getColor4f("finish", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinishVariance = color.getColor4f("finish_variance", { 1.0f, 1.0f, 1.0f, 1.0f });
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
    }

    component->texture = json.getString("texture", "");

    return (component);
}

JsonValue&    ComponentFactory<sParticleEmitterComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue color;
    JsonValue size;
    JsonValue& json = _componentsJson[entityType];
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(_components[entityType]);

    // Write colors
    color.setColor4f("start", component->colorStart);
    color.setColor4f("start_variance", component->colorStartVariance);
    color.setColor4f("finish", component->colorFinish);
    color.setColor4f("finish_variance", component->colorFinishVariance);
    json.setValue("color", color);

    // Write size
    size.setFloat("start", component->sizeStart);
    size.setFloat("start_variance", component->sizeStartVariance);
    size.setFloat("finish", component->sizeFinish);
    size.setFloat("finish_variance", component->sizeFinishVariance);
    json.setValue("size", size);

    json.setFloat("rate", component->rate);
    json.setUInt("spawn_nb", component->spawnNb);
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
    changed |= ImGui::SliderInt("Particles number per spawn", (int*)&component->spawnNb, 0, 50);
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
** sTowerAIComponent
*/

sComponent* ComponentFactory<sTowerAIComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTowerAIComponent*  component;

    component = new sTowerAIComponent();
    component->radius = json.getFloat("radius", 1.0f);
    component->fireRate = json.getFloat("fire_rate", 1.0f);
    component->projectileSpeed = json.getFloat("projectile_speed", 1.0f);

    return (component);
}

JsonValue&  ComponentFactory<sTowerAIComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue&  json = _componentsJson[entityType];
    sTowerAIComponent*  component;

    component = static_cast<sTowerAIComponent*>(_components[entityType]);
    json.setFloat("radius", component->radius);
    json.setFloat("fire_rate", component->fireRate);
    json.setFloat("projectile_speed", component->projectileSpeed);
    return (json);
}

bool    ComponentFactory<sTowerAIComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sTowerAIComponent*  component;
    bool    changed;

    component = static_cast<sTowerAIComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    changed = false;
    changed |= ImGui::SliderFloat("Radius", &component->radius, 0.0f, 10.0f);
    changed |= ImGui::SliderFloat("Fire rate", &component->fireRate, 0.0f, 10.0f);
    changed |= ImGui::SliderFloat("Projectile speed", &component->projectileSpeed, 0.0f, 10.0f);

    return (changed);
}


/*
** sProjectileComponent
*/

sComponent* ComponentFactory<sProjectileComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sProjectileComponent*   component;

    component = new sProjectileComponent();
    component->shooterId = 0;
    component->guided = json.getBool("guided", false);
    component->rangeMax = json.getFloat("rangeMax", 5);
    component->targetId = 0;
    return (component);
}

JsonValue&  ComponentFactory<sProjectileComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue&              json = _componentsJson[entityType];
    sProjectileComponent*   component;

    component = static_cast<sProjectileComponent*>(_components[entityType]);
    json.setBool("guided", component->guided);
    return (json);
}

bool        ComponentFactory<sProjectileComponent>::updateEditor(const std::string& entityType, sComponent** savedComponent, sComponent* entityComponent, Entity* entity)
{
    sProjectileComponent*   component;
    bool                    changed;
    std::stringstream       converter;
    std::string             buffer;

    component = static_cast<sProjectileComponent*>(entityComponent ? entityComponent : _components[entityType]);
    *savedComponent = component;
    changed = false;
    converter << component->shooterId;
    converter >> buffer;
    ImGui::TextDisabled("Shooter ID: %s", buffer.c_str());
    if (component->guided == true) ImGui::Separator();
    changed |= ImGui::Checkbox("Make projectile guided !", &component->guided);
    if (component->guided == true)
    {
        converter << component->targetId;
        converter >> buffer;
        ImGui::TextDisabled("Target ID: %s", buffer.c_str());
    }
    // Add IMGUI implementation for RangeMax

    return (changed);
}


/*
** sWaveComponent
*/

sComponent* ComponentFactory<sWaveComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sWaveComponent*   component;

    component = new sWaveComponent();

    //component->spawnPos = json.getVec3f("spawnPos", glm::vec3(0, 0, 0));
    component->secBeforeSpawn = json.getFloat("secBeforeSpawn", 2);
    component->nSpawn = json.getInt("nSpawn", 10);

    return (component);
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

JsonValue&    ComponentFactory<sTransformComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sTransformComponent* component = static_cast<sTransformComponent*>(_components[entityType]);


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

    ComponentFactory<sTransformComponent>::updateTransforms(component->pos,
                                                            component->scale,
                                                            component->rotation,
                                                            component->transform,
                                                            ImGuizmo::LOCAL);

    return (false);
}

bool    ComponentFactory<sTransformComponent>::updateTransforms(glm::vec3& pos, glm::vec3& scale, glm::vec3& rotation, glm::mat4& transform, ImGuizmo::MODE mode)
{
    auto &&keyboard = GameWindow::getInstance()->getKeyboard();
    Camera* camera = Camera::getInstance();
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE) || keyboard.isPressed(Keyboard::eKey::T))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE) || keyboard.isPressed(Keyboard::eKey::R))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE) || keyboard.isPressed(Keyboard::eKey::E))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale));
    ImGui::InputFloat3("Translate", glm::value_ptr(pos), 3);
    ImGui::InputFloat3("Rotation", glm::value_ptr(rotation), 3);
    ImGui::InputFloat3("Scale", glm::value_ptr(scale), 3);
    ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale), glm::value_ptr(transform));

    ImGuizmo::Manipulate(glm::value_ptr(camera->getView()), glm::value_ptr(camera->getProj()), mCurrentGizmoOperation, mode, glm::value_ptr(transform), nullptr, nullptr);
    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(pos), glm::value_ptr(rotation), glm::value_ptr(scale));

    return (false);
}
