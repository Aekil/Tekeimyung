#pragma once

#include <cstdint>
#include <memory>

class Entity;
class EntityManager;

class EntityPool {
private:
    struct Chunk {
        Entity* entities;

        // Store indices of free entities
        std::vector<uint32_t> freeEntities;

        // Chunk idx
        uint32_t idx;
    };

public:
    EntityPool(EntityManager* em, uint32_t entitiesPerChunk);
    ~EntityPool();

    Entity*                             allocate();
    void                                free(Entity* entity);

    Entity*                             getEntity(const Entity::sHandle& handle);

private:
    EntityManager*                      _em;

    uint32_t                            _entitiesPerChunk;
    std::vector<std::unique_ptr<Chunk> > _chunks;
};
