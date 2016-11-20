#include <algorithm>
#include <sstream>
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Window/Keyboard.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Game/ComponentFactory.hpp>

std::unordered_map<std::string, IComponentFactory*>  IComponentFactory::_componentsTypes = { COMPONENTS_TYPES() };


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

/*
** sRenderComponent
*/

sComponent* ComponentFactory<sRenderComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sRenderComponent* component = new sRenderComponent();
    JsonValue geometry(json.get("geometry", {}));

    // Initialize some values
    component->animated = json.getBool("animated", false);
    component->modelFile = json.getString("model", "resources/models/default.DAE");
    component->color = json.getColor4f("color", { 1.0f, 1.0f, 1.0f, 1.0f });
    component->geometry = Geometry::eType::MESH;

    // Pre-initialize the values so that the values are not 0 when changing the geometry type
    component->geometryInfo.plane.width = 30.0f;
    component->geometryInfo.plane.height = 30.0f;
    component->geometryInfo.cube.size = 10.0f;

    if (geometry.size() > 0)
    {
        std::string geometryName = geometry.getString("name", "");
        component->geometry = Geometry::getGeometryType(geometryName);

        if (component->geometry == Geometry::eType::PLANE)
        {
            component->geometryInfo.plane.width = geometry.getFloat("width", 30.0f);
            component->geometryInfo.plane.height = geometry.getFloat("height", 30.0f);
        }
        else if (component->geometry == Geometry::eType::CUBE)
        {
            component->geometryInfo.cube.size = geometry.getFloat("size", 10.0f);
        }
        else
            EXCEPT(InvalidParametersException, "Unknown geometry name \"%s\" for sRenderComponent of \"%s\"", geometryName.c_str(), entityType.c_str());
    }

    return (component);
}

JsonValue&    ComponentFactory<sRenderComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    JsonValue geometry;
    sRenderComponent* component = static_cast<sRenderComponent*>(_components[entityType]);


    json.setBool("animated", component->animated);
    json.setString("model", component->modelFile);
    json.setColor4f("color", component->color);

    if (component->geometry != Geometry::eType::MESH)
    {
        geometry.setString("name", Geometry::getGeometryTypeString(component->geometry));
        if (component->geometry == Geometry::eType::PLANE)
        {
            geometry.setFloat("width", component->geometryInfo.plane.width);
            geometry.setFloat("height", component->geometryInfo.plane.height);
        }
        else if (component->geometry == Geometry::eType::CUBE)
        {
            geometry.setFloat("size", component->geometryInfo.cube.size);
        }
    }

    json.setValue("geometry", geometry);
    return (json);
}

bool    ComponentFactory<sRenderComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sRenderComponent* component = static_cast<sRenderComponent*>(_components[entityType]);
    *component_ = component;
    bool changed = false;
    bool typeChanged = false;

    ImGui::PushItemWidth(200);
    if (ImGui::CollapsingHeader("sRenderComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        changed |= ImGui::ColorEdit3("color", glm::value_ptr(component->color));

        static std::vector<const char*>& typesString = const_cast<std::vector<const char*>&>(Geometry::getTypesString());
        int selectedType = static_cast<int>(std::find(typesString.cbegin(), typesString.cend(), Geometry::getGeometryTypeString(component->geometry)) - typesString.begin());
        const char** typesList = typesString.data();

        typeChanged = ImGui::ListBox("Model type", &selectedType, typesList, (int)Geometry::getTypesString().size(), 4);
        if (typeChanged)
            component->geometry = Geometry::getGeometryType(typesString[selectedType]);

        // Plan
        if (component->geometry == Geometry::eType::PLANE)
        {
            changed |= ImGui::SliderFloat("width", &component->geometryInfo.plane.width, 1.0f, 100.0f);
            changed |= ImGui::SliderFloat("height", &component->geometryInfo.plane.height, 1.0f, 100.0f);
        }
        // CUBE
        else if (component->geometry == Geometry::eType::CUBE)
        {
            changed |= ImGui::SliderFloat("size", &component->geometryInfo.cube.size, 1.0f, 100.0f);
        }
        // MESH
        else if (component->geometry == Geometry::eType::MESH)
        {
            static RessourceManager* resourceManager = RessourceManager::getInstance();
            auto model = resourceManager->getModel(component->modelFile);
            static std::vector<const char*>& modelsString = const_cast<std::vector<const char*>&>(resourceManager->getModelsNames());
            int selectedModel = static_cast<int>(std::find(modelsString.cbegin(), modelsString.cend(), model->getId()) - modelsString.begin());
            const char** modelsList = modelsString.data();

            if (ImGui::ListBox("Model", &selectedModel, modelsList, (int)resourceManager->getModelsNames().size(), 4))
            {
                changed = true;
                model = resourceManager->getModel(modelsString[selectedModel]);
                component->modelFile = model->getPath();
            }
        }

        changed |= typeChanged;
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
** sRectHitboxComponent
*/

sComponent* ComponentFactory<sRectHitboxComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sRectHitboxComponent* component = new sRectHitboxComponent();

    component->min = json.getVec2f("min", { 0.0f, 0.0f });
    component->max = json.getVec2f("max", { 0.0f, 0.0f });

    return (component);
}

JsonValue&    ComponentFactory<sRectHitboxComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sRectHitboxComponent* component = static_cast<sRectHitboxComponent*>(_components[entityType]);

    json.setVec2f("min", component->min);
    json.setVec2f("max", component->max);

    return (json);
}


/*
** sCircleHitboxComponent
*/

sComponent* ComponentFactory<sCircleHitboxComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sCircleHitboxComponent* component = new sCircleHitboxComponent();

    component->center = json.getVec2f("center", { 0.0f, 0.0f });
    component->radius = json.getFloat("radius", 0.0f);

    return (component);
}

JsonValue&    ComponentFactory<sCircleHitboxComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sCircleHitboxComponent* component = static_cast<sCircleHitboxComponent*>(_components[entityType]);

    json.setVec2f("center", component->center);
    json.setFloat("radius", component->radius);

    return (json);
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
    component->spawnNb = json.getUInt("spawn_nb", 0);
    component->emitterLife = json.getFloat("emitter_life", 0.0f);
    component->life = json.getUInt("life", 0);
    component->lifeVariance = json.getUInt("life_variance", 0);
    component->angle = json.getFloat("angle", 0.0f);
    component->angleVariance = json.getFloat("angle_variance", 0.0f);
    component->speed = json.getFloat("speed", 0.0f);
    component->speedVariance =  json.getFloat("speed_variance", 0.0f);

    if (color.size() > 0)
    {
        component->colorStart = color.getColor4f("start", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorStartVariance = color.getColor4f("start_variance", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinish = color.getColor4f("finish", { 1.0f, 1.0f, 1.0f, 1.0f });
        component->colorFinishVariance = color.getColor4f("finish_variance", { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    if (size.size() > 0)
    {
        component->sizeStart =  size.getFloat("start", 1.0f);
        component->sizeFinish =  size.getFloat("finish", 1.0f);
        component->sizeStartVariance =  size.getFloat("start_variance", 1.0f);
        component->sizeFinishVariance =  size.getFloat("finish_variance", 1.0f);
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

bool    ComponentFactory<sParticleEmitterComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sParticleEmitterComponent* component = static_cast<sParticleEmitterComponent*>(_components[entityType]);
    *component_ = component;
    bool changed = false;

    ImGui::PushItemWidth(200);
    if (ImGui::CollapsingHeader("sParticleEmitterComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
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
    }

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

bool    ComponentFactory<sTowerAIComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sTowerAIComponent*  component;
    bool    changed;

    component = static_cast<sTowerAIComponent*>(_components[entityType]);
    *component_ = component;
    changed = false;
    if (ImGui::CollapsingHeader("sTowerAIComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        changed |= ImGui::SliderFloat("Radius", &component->radius, 0.0f, 10.0f);
        changed |= ImGui::SliderFloat("Fire rate", &component->fireRate, 0.0f, 10.0f);
        changed |= ImGui::SliderFloat("Projectile speed", &component->projectileSpeed, 0.0f, 10.0f);
    }

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

bool        ComponentFactory<sProjectileComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sProjectileComponent*   component;
    bool                    changed;
    std::stringstream       converter;
    std::string             buffer;

    component = static_cast<sProjectileComponent*>(_components[entityType]);
    *component_ = component;
    changed = false;
    if (ImGui::CollapsingHeader("sProjectileComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
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
    }

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
** sTransformComponent
*/

sComponent* ComponentFactory<sTransformComponent>::loadFromJson(const std::string& entityType, const JsonValue& json)
{
    sTransformComponent*   component;

    component = new sTransformComponent();

    component->scale = json.getVec3f("scale", { 1.0f, 1.0f, 1.0f });
    component->pos = json.getVec3f("pos", { 0.0f, 0.0f, 0.0f });

    return (component);
}

JsonValue&    ComponentFactory<sTransformComponent>::saveToJson(const std::string& entityType, const std::string& componentType)
{
    JsonValue& json = _componentsJson[entityType];
    sTransformComponent* component = static_cast<sTransformComponent*>(_components[entityType]);


    json.getVec3f("scale", component->scale);
    json.getVec3f("pos", component->pos);

    return (json);
}

bool    ComponentFactory<sTransformComponent>::updateEditor(const std::string& entityType, sComponent** component_)
{
    sTransformComponent* component = static_cast<sTransformComponent*>(_components[entityType]);
    *component_ = component;
    bool changed = false;
    bool scaleChanged = false;

    ImGui::PushItemWidth(200);
    if (ImGui::CollapsingHeader("sTransformComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        float scale = component->scale.x;

        changed |= ImGui::SliderFloat3("scale",  glm::value_ptr(component->scale), 0.1f, 200.0f);
        changed |= ImGui::SliderFloat3("pos",  glm::value_ptr(component->pos), 0.1f, 200.0f);
        scaleChanged |= ImGui::SliderFloat("scale all",  &scale, 0.1f, 200.0f);

        if (scaleChanged)
        {
            component->scale.x = scale;
            component->scale.y = scale;
            component->scale.z = scale;
        }
    }

    return (changed || scaleChanged);
}
