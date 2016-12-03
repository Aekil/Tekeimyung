#include <algorithm>
#include <iostream>

#include <Game/ComponentFactory.hpp>
#include <dirent.h> // This include have to be called after "ComponentFactory.hpp"

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Game/Utils/JsonReader.hpp>

#include <Game/EntityFactory.hpp>

std::unordered_map<std::string, std::list<std::string> >  EntityFactory::_entities;
std::unordered_map<std::string, std::string>  EntityFactory::_entitiesFiles;
std::vector<const char*>  EntityFactory::_typesString = { ENTITIES_TYPES(GENERATE_STRING) };
EntityManager*  EntityFactory::_em = nullptr;

EntityFactory::EntityFactory() {}

EntityFactory::~EntityFactory() {}

void EntityFactory::loadDirectory(const std::string& archetypesDir)
{
    DIR* dir;
    struct dirent* ent;
    RessourceManager* ressourceManager = RessourceManager::getInstance();

    dir = opendir(archetypesDir.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open archetypes directory \"%s\"", archetypesDir.c_str());

    while ((ent = readdir(dir)) != NULL)
    {
        if (RessourceManager::getFileExtension(ent->d_name) == "json")
        {
            // Get entity configuration file
            std::string path = std::string(archetypesDir).append("/").append(ent->d_name);
            std::string entityConf = ressourceManager->getFile(path);

            // Parse the configuration file with jsoncpp
            JsonReader jsonReader;
            JsonValue parsed;
            std::string typeName;
            if (!jsonReader.parse(entityConf, parsed))
                EXCEPT(IOException, "Cannot parse archetype \"%s\"", path.c_str());

            typeName = parsed.getString("name", "");
            LOG_INFO("Load entity %s", typeName.c_str());

            if (!EntityFactory::entityTypeExists(typeName)) // The macro ENTITIES_TYPES did not create the type
                EXCEPT(InvalidParametersException, "Failed to read entity archetype: Entity type \"%s\" does not exist", typeName.c_str());

            _entitiesFiles[typeName] = path;

            // Create entity components
            auto &&components = parsed.get("components", {}).get();
            for (Json::ValueIterator it = components.begin(); it != components.end(); it++)
            {
                std::string componentName = it.key().asString();

                // The macro COMPONENTS_TYPES did not create the type
                if (!IComponentFactory::componentTypeExists(it.key().asString()))
                    EXCEPT(InvalidParametersException, "Failed to read entity archetype: Component type \"%s\" does not exist", componentName.c_str());

                LOG_INFO("Add %s component %s", typeName.c_str(), componentName.c_str());
                IComponentFactory::initComponent(typeName, componentName, JsonValue(*it));
                _entities[typeName].push_back(componentName);
            }

            // Add sTransformation component if not found
            if (std::find(_entities[typeName].begin(), _entities[typeName].end(), "sTransformComponent") == _entities[typeName].end())
            {
                IComponentFactory::initComponent(typeName, "sTransformComponent", {});
                _entities[typeName].push_back("sTransformComponent");
            }
        }
        // No file extension, is directory
        else if (std::string(ent->d_name).find(".") == std::string::npos)
        {
            // Load directory
            std::string directoryPath = std::string(archetypesDir).append("/").append(ent->d_name);
            loadDirectory(directoryPath);
        }
    }

    closedir(dir);
}

bool    EntityFactory::entityTypeExists(const std::string& type)
{
    for (auto &&entityType: _typesString)
    {
        if (entityType == type)
            return (true);
    }

    return (false);
}

Entity* EntityFactory::createEntity(eArchetype type)
{
    if ((int)type > (int)_typesString.size() - 1)
    {
        EXCEPT(InvalidParametersException, "The entity type does not exist");
        return (nullptr);
    }

    ASSERT(_em != nullptr, "The entity manager should not be null");

    std::string typeName = _typesString[(int)type];

    return (cloneEntity(typeName));
}

Entity* EntityFactory::createEntity(const std::string& typeName)
{
    return (cloneEntity(typeName));
}

void    EntityFactory::createEntityType(const std::string& typeName)
{
    // Add entity to factory
    std::string filePath = ARCHETYPES_LOCATION + std::string("/") + Helper::lowerCaseString(typeName) + ".json";
    _entitiesFiles[typeName] = filePath;
    _typesString.push_back(_entitiesFiles.find(typeName)->first.c_str());

    // Add transform component
    IComponentFactory::initComponent(typeName, "sTransformComponent", {});
    _entities[typeName].push_back("sTransformComponent");

    // Create file
    RessourceManager::getInstance()->createFile(filePath, "{\"name\": \"" + typeName + "\"}");

    // Create entity instance to edit
    createEntity(typeName);
}

void EntityFactory::bindEntityManager(EntityManager* em)
{
    _em = em;
}

const std::vector<const char*>& EntityFactory::getTypesString()
{
    return _typesString;
}

const std::list<std::string>&   EntityFactory::getComponents(const std::string& typeName)
{
    return _entities[typeName];
}

const void   EntityFactory::removeComponent(const std::string& typeName, const std::string& component)
{
    auto& components = _entities[typeName];
    auto foundComponent = std::find(components.begin(), components.end(), component);
    if (foundComponent != components.end())
    {
        components.erase(foundComponent);
    }
}

const void   EntityFactory::addComponent(const std::string& typeName, const std::string& component)
{
    _entities[typeName].push_back(component);
}

const std::string& EntityFactory::getFile(const std::string& typeName)
{
    return _entitiesFiles[typeName];
}

Entity* EntityFactory::cloneEntity(const std::string& typeName)
{
    Entity* clone = _em->createEntity();
    auto entityComponents = _entities.find(typeName);

    if (entityComponents == _entities.end())
        EXCEPT(InvalidParametersException, "The entity type %s does not exist", typeName);

    for (auto &&component: entityComponents->second)
    {
        sComponent* component_ = IComponentFactory::createComponent(typeName, component);
        clone->addComponent(component_);
    }

    clone->addComponent<sNameComponent>(typeName);

    initAnimations(clone);

    return (clone);
}

void    EntityFactory::initAnimations(Entity* entity)
{
    sRenderComponent* render = entity->getComponent<sRenderComponent>();
    sTransformComponent* transform = entity->getComponent<sTransformComponent>();

    if (!render)
        return;

    for (auto animation: render->_animations)
    {
        for (auto paramAnimation: animation->getParamsAnimations())
        {
            if (transform && paramAnimation->getName() == "position")
                std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation)->setParam(&transform->pos);
            else if (transform && paramAnimation->getName() == "rotation")
                std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation)->setParam(&transform->rotation);
            else if (transform && paramAnimation->getName() == "scale")
                std::static_pointer_cast<ParamAnimation<glm::vec3>>(paramAnimation)->setParam(&transform->scale);
            else if (transform && paramAnimation->getName() == "color")
                std::static_pointer_cast<ParamAnimation<glm::vec4>>(paramAnimation)->setParam(&render->color);
        }
    }
}

void    EntityFactory::updateEntityComponent(const std::string& entityName, IComponentFactory* compFactory, sComponent* component)
{
    for (auto &&entity_: _em->getEntities())
    {
        Entity* entity = entity_.second;
        sNameComponent* name = entity->getComponent<sNameComponent>();

        if (name->value == entityName)
        {
            sComponent* entityComponent = entity->getComponent(component->id);
            if (entityComponent && entityComponent->id != sPositionComponent::identifier)
            {
                // Only the scale have to be copied
                if (entityComponent->id == sTransformComponent::identifier)
                {
                    sTransformComponent* transform = static_cast<sTransformComponent*>(component);
                    sTransformComponent* entityTransform = static_cast<sTransformComponent*>(entityComponent);

                    entityTransform->scale = transform->scale;
                    entityTransform->needUpdate = true;
                }
                else
                {
                    entityComponent->update(component);
                }
            }
        }
    }
}
