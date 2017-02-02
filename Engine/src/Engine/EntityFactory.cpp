/**
* @Author   Guillaume Labey
*/

#include <algorithm>
#include <iostream>

#include <Engine/ComponentFactory.hpp>
#include <dirent.h> // This include has to be called after "ComponentFactory.hpp"

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/Helper.hpp>
#include <Engine/Utils/Exception.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Utils/JsonReader.hpp>
#include <Engine/Utils/JsonWriter.hpp>

#include <Engine/EntityFactory.hpp>

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

    dir = opendir(archetypesDir.c_str());
    if (!dir)
        EXCEPT(FileNotFoundException, "Cannot open archetypes directory \"%s\"", archetypesDir.c_str());

    while ((ent = readdir(dir)) != NULL)
    {
        if (RessourceManager::getFileExtension(ent->d_name) == "json")
        {
            // Get entity configuration file
            std::string path = std::string(archetypesDir).append("/").append(ent->d_name);

            // Parse the configuration file with jsoncpp
            JsonReader jsonReader;
            JsonValue parsed;
            std::string typeName;
            if (!jsonReader.parse(path, parsed))
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

            if (std::find(_entities[typeName].begin(), _entities[typeName].end(), "sNameComponent") == _entities[typeName].end())
                EXCEPT(InvalidParametersException, "Failed to read entity archetype \"%s\": missing sNameComponent", typeName.c_str());

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
    for (auto &&entityType : _typesString)
    {
        if (entityType == type)
            return (true);
    }

    return (false);
}

Entity* EntityFactory::createOrGetEntity(eArchetype type)
{
    std::string typeName = _typesString[(int)type];
    for (auto &&entity_ : _em->getEntities())
    {
        Entity* entity = entity_.second;
        sNameComponent* name = entity->getComponent<sNameComponent>();

        ASSERT(name != nullptr, "The entity should have a sNameComponent");

        if (name->value == typeName)
        {
            return (entity);
        }
    }

    return (createEntity(type));
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

Entity* EntityFactory::createEntity(eArchetype type, const glm::vec3& pos)
{
    Entity* entity = createEntity(type);

    sTransformComponent* transformEntity = entity->getComponent<sTransformComponent>();
    transformEntity->pos = pos;
    transformEntity->needUpdate = true;

    return (entity);
}

Entity* EntityFactory::createEntity(const std::string& typeName, const glm::vec3& pos)
{
    Entity *entity = cloneEntity(typeName);

    sTransformComponent* transform = entity->getComponent<sTransformComponent>();

    transform->pos = pos;

    return (entity);
}

Entity* EntityFactory::createEntity(const std::string& typeName)
{
    return (cloneEntity(typeName));
}

void EntityFactory::createEntityType(const std::string& typeName)
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
}

void EntityFactory::bindEntityManager(EntityManager* em)
{
    _em = em;
}

EntityManager*  EntityFactory::getBindedEntityManager()
{
    return _em;
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
        EXCEPT(InvalidParametersException, "The entity type %s does not exist", typeName.c_str());

    for (auto &&component : entityComponents->second)
    {
        sComponent* component_ = IComponentFactory::createComponent(typeName, component);
        clone->addComponent(component_);
    }

    initAnimations(clone);

    return (clone);
}

void    EntityFactory::copyEntityManager(EntityManager* dst, EntityManager* src)
{
    auto& entities = src->getEntities();
    for (auto& entity: entities)
    {
        Entity* cloneEntity = dst->createEntity();
        auto& components = entity.second->getComponents();
        for (auto& component : components)
        {
            sComponent* cloneComponent = component->clone();
            cloneEntity->addComponent(cloneComponent);
        }
    }
}

void    EntityFactory::initAnimations(Entity* entity)
{
    sRenderComponent* render = entity->getComponent<sRenderComponent>();

    if (!render || render->_animator.getAnimationsNb() == 0)
        return;

    sTransformComponent* transform = entity->getComponent<sTransformComponent>();

    for (auto animation : render->_animator.getAnimations())
    {
        for (auto paramAnimation : animation->getParamsAnimations())
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

void    EntityFactory::reverseAnimations(Entity* entity)
{
    sRenderComponent* render = entity->getComponent<sRenderComponent>();
    if (render && render->_animator.isPlaying())
    {
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        render->_animator.reset();
        render->_animator.update(0);
        transform->updateTransform();
    }
}

void    EntityFactory::updateEntitiesComponents(Entity* from, const std::string& entityName, IComponentFactory* compFactory, sComponent* component)
{
    for (auto &&entity_ : _em->getEntities())
    {
        Entity* entity = entity_.second;
        sNameComponent* name = entity->getComponent<sNameComponent>();

        if (name->value == entityName && entity != from)
        {
            sComponent* entityComponent = entity->getComponent(component->id);

            // Add component to entity
            if (!entityComponent)
            {
                entity->addComponent(component->clone());
            }
            // Update component
            else
            {
                // Reverse animations if we are overwriting them
                if (component->id == sRenderComponent::identifier)
                {
                    reverseAnimations(entity);
                }

                if (component->id == sRenderComponent::identifier)
                {

                    initAnimations(entity);

                    sRenderComponent* render = static_cast<sRenderComponent*>(entityComponent);

                    if (render && render->_animator.getAnimationsNb() > 0)
                    {
                        AnimationPtr currentAnimation = render->_animator.getAnimations()[0];
                        render->_animator.play(currentAnimation->getName());
                    }
                }
            }
        }
    }
}

void    EntityFactory::saveEntityTemplateToJson(const std::string& typeName)
{
    JsonWriter jsonWriter;
    JsonValue json;
    JsonValue components;
    std::string savedFile = EntityFactory::getFile(typeName);
    auto&& entitycomponents = EntityFactory::getComponents(typeName);

    json.setString("name", typeName);
    for (auto &&component: entitycomponents)
    {
        JsonValue& componentJson = IComponentFactory::getFactory(component)->saveToJson(typeName);
        components.setValue(component, componentJson);
    }
    json.setValue("components", components);

    jsonWriter.write(savedFile, json);

    LOG_INFO("Entity template %s saved to %s", typeName.c_str(), savedFile.c_str());
}

void    EntityFactory::saveEntityTemplate(const std::string& typeName, Entity* entity)
{
    {
        auto& components = EntityFactory::getComponents(typeName);
        // Remove deleted components from factories
        for (auto it = components.begin(); it != components.end();)
        {
            auto component = *it;
            std::size_t componentHash = IComponentFactory::getComponentHashWithName(component);
            auto compFactory = IComponentFactory::getFactory(component);
            ASSERT(compFactory != nullptr, "The factory should exist");

            // The component has been removed, delete it from EntityFactory and ComponentFactory
            if (!entity->hasComponent(componentHash))
            {
                ++it;
                EntityFactory::removeComponent(typeName, component);
                compFactory->remove(typeName);
            }
            else
                ++it;
        }
    }

    {
        auto& components = EntityFactory::getComponents(typeName);

        // Reverse animations
        EntityFactory::reverseAnimations(entity);

        // Save entity components
        for (auto component: entity->getComponents())
        {
            std::string componentName = IComponentFactory::getComponentNameWithHash(component->id);
            auto compFactory = IComponentFactory::getFactory(componentName);
            ASSERT(compFactory != nullptr, "The factory should exist");

            compFactory->save(typeName, component);

            // The component does not exist in entityFactory
            if (std::find(components.begin(), components.end(), componentName) == components.end())
            {
                // Add component to EntityFactory
                EntityFactory::addComponent(typeName, componentName);
            }

            // Update other entities component
            EntityFactory::updateEntitiesComponents(entity, typeName, compFactory, component);
        }
    }

    {
        // Remove entities components if entity does not has the component
        for (auto &&entityIt : _em->getEntities())
        {
            Entity* entity_ = entityIt.second;
            sNameComponent* name = entity_->getComponent<sNameComponent>();

            if (name->value == typeName && entity_ != entity)
            {
                auto& components = entity_->getComponents();
                auto& it = components.begin();
                for (it; it != components.end();)
                {
                    sComponent* component = *it;
                    if (!entity->hasComponent(component->id))
                    {
                        delete component;
                        it = components.erase(it);
                    }
                    else
                        ++it;
                }
            }
        }
    }

    LOG_INFO("Entity template %s overwritten by entity", typeName.c_str());
}
