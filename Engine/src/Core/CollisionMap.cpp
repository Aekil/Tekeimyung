#include <cstring>
#include "Core/CollisionMap.hpp"

/*
** LineReference
*/

CollisionMap::LineReference::LineReference(CollisionMap *map, uint16_t layerIdx, uint32_t lineIdx):
    _map(map), _layerIdx(layerIdx), _lineIdx(lineIdx) {}

CollisionMap::Square&   CollisionMap::LineReference::operator[](unsigned int idx)
{
    // Collision square need update
    if (_map->_collisionMap[_layerIdx][_lineIdx][idx] == eColType::DIRTY)
    {
        Entity* entity = _map->_em.getEntity((*_map->_map)[_layerIdx][_lineIdx][idx].get());
        sTypeComponent* entityType = entity ? entity->getComponent<sTypeComponent>() : nullptr;

        if (entityType)
        {
            _map->_collisionMap[_layerIdx][_lineIdx][idx] = _map->colTypeFromEntityType(entityType->type);
        }
        else
        {
            _map->_collisionMap[_layerIdx][_lineIdx][idx] = eColType::CAN_WALK;
        }
    }

    return _map->_collisionMap[_layerIdx][_lineIdx][idx];
}



/*
** LayerReference
*/

CollisionMap::LayerReference::LayerReference(CollisionMap* map, unsigned int idx): _map(map), _layerIdx(idx) {}

CollisionMap::LineReference  CollisionMap::LayerReference::operator[](unsigned int idx)
{
    return LineReference(_map, _layerIdx, idx);
}


CollisionMap::CollisionMap(EntityManager& em, Map* map): _map(map), _em(em)
{
    _collisionMap = new CollisionMap::Layer[_map->getLayersNb()];

    for (uint16_t layer = 0; layer < _map->getLayersNb(); layer++)
    {
        _collisionMap[layer] = new CollisionMap::Square*[_map->getHeight()];
        for (uint32_t y = 0; y < _map->getHeight(); y++)
        {
            _collisionMap[layer][y] = new CollisionMap::Square[_map->getWidth()];

            std::memset(_collisionMap[layer][y], 0, sizeof(_collisionMap[layer][y][0]) * _map->getWidth());
        }
    }
}

CollisionMap::~CollisionMap()
{
    delete _collisionMap;
}

CollisionMap::LayerReference    CollisionMap::operator[](unsigned int idx)
{
    return LayerReference(this, idx);
}

eColType                        CollisionMap::colTypeFromEntityType(eEntityType entityType) const
{
    switch(entityType)
    {
        case eEntityType::CHARACTER:
            return eColType::CAN_WALK;
        case eEntityType::OBJECT:
            return eColType::CAN_NOT_WALK;
        case eEntityType::TILE:
            return eColType::CAN_NOT_WALK;
        case eEntityType::TILE_STAIRS_UP:
            return eColType::GO_LAYER_UP;
        case eEntityType::TILE_STAIRS_DOWN:
            return eColType::GO_LAYER_DOWN;
    }

    //TODO: Add assert

    return eColType::DIRTY;
}
