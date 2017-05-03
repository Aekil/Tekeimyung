#include <ECS/Entity.hpp>
#include <ECS/EntityPool.hpp>

EntityPool::EntityPool(EntityManager* em, uint32_t entitiesPerChunk): _em(em), _entitiesPerChunk(entitiesPerChunk) {}

EntityPool::~EntityPool()
{
    for (const auto& chunk: _chunks) {
        delete[] chunk->entities;
    }
}

Entity* EntityPool::allocate()
{
    // Search free entity
    {
        for (const auto& chunk: _chunks) {
            if (chunk->freeEntities.size() != 0)
            {
                Entity* entity = chunk->entities + chunk->freeEntities.back();
                entity->_free = false;
                // Update handle version
                // All previous handles will be invalidated
                if (entity->handle.count == ENTITY_HANDLE_MAX_COUNT)
                {
                    entity->handle.count = 0;
                }
                else
                {
                    entity->handle.count++;
                }
                chunk->freeEntities.pop_back();
                return (entity);
            }
        }
    }

    // No free sub-buffer found, allocate new chunk
    std::unique_ptr<EntityPool::Chunk> chunk = std::make_unique<EntityPool::Chunk>();
    chunk->idx = (uint32_t)_chunks.size();

    // Init new entities chunk
    {
        chunk->entities = new Entity[_entitiesPerChunk];
        chunk->freeEntities.resize(_entitiesPerChunk);
        for (uint32_t i = 0; i < _entitiesPerChunk; ++i) {
            // We do +1 on id because we want to use 0 id as null entity
            // So we can do getEntity(0)
            chunk->entities[i].handle.index = chunk->idx * _entitiesPerChunk + i + 1;
            chunk->freeEntities[i] = i;
            chunk->entities[i]._em = _em;
            chunk->entities[i]._free = true;
        }
    }

    _chunks.push_back(std::move(chunk));

    return (allocate());
}

void EntityPool::free(Entity* entity)
{
    // Remember ID is ID + 1
    uint32_t entityId = entity->handle.index - 1;

    uint32_t chunkNb = entityId / _entitiesPerChunk;
    entity->_free = true;

    auto& chunk = _chunks[chunkNb];
    chunk->freeEntities.insert(chunk->freeEntities.begin(), entityId % _entitiesPerChunk);
}

Entity* EntityPool::getEntity(const Entity::sHandle& handle)
{
    // Remember ID is ID + 1
    uint32_t chunkNb = (handle.index - 1) / _entitiesPerChunk;

    if (chunkNb >= _chunks.size() || handle.index == 0)
    {
        return (nullptr);
    }

    Entity* entity = _chunks[chunkNb]->entities + ((handle.index - 1) % _entitiesPerChunk);

    if (entity->_free || // The entity is not allocated
        entity->handle != handle) // Outdated handle
        return (nullptr);

    return (entity);
}
