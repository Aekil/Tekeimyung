/**
* @Author   Guillaume Labey
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <ECS/Entity.hpp>
#include <ECS/EntityManager.hpp>
#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/JsonValue.hpp>
#include <Engine/Core/GameState.hpp>

#define LEVELS_DIRECTORY    "resources/levels/"

#define REGISTER_GAMESTATE(NAME)                                    \
    LevelLoader::getInstance()->addLoadedState<NAME##Factory>();

class LevelLoader
{
 public:
    LevelLoader();
    ~LevelLoader();

    static LevelLoader*                 getInstance();

    const std::vector<std::string>&     getLevels() const;
    std::vector<std::string>&           getLevels();

    const std::vector<std::unique_ptr<GameStateFactory> >& getLoadedStates();

    void                                save(const std::string& levelName, const std::unordered_map<uint32_t, Entity*>& entities);
    void                                load(const std::string& levelName, EntityManager* em);

    void                                loadDirectory(const std::string& directory);

    template <typename T>
    void                                addLoadedState()
    {
        if (T::identifier == EditorState::identifier)
        {
            ASSERT(true, "The EditorState should not be registered");
        }
        _loadedStates.push_back(std::make_unique<T>());
    }

private:
    void                                addComponentToEntity(Entity* entity, const std::string& entityTypeName, const JsonValue& componentJson, const std::string& componentName);

private:
    std::vector<std::string>            _levels;
    std::vector<std::unique_ptr<GameStateFactory> > _loadedStates;

    static std::unique_ptr<LevelLoader> _instance;
};
