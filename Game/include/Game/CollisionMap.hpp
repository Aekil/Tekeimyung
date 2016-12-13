/**
* @Author   Guillaume Labey
*/

#pragma once

#include <cstdint>
#include <cstddef>

#include <Game/Map.hpp>
#include <Game/Components.hh>

enum class eColType: char
{
    DIRTY = 0, // Need update
    CAN_WALK = 1,
    CAN_NOT_WALK = 2,
    GO_LAYER_UP = 3,
    GO_LAYER_DOWN = 4

};


class Map;
class EntityManager;

class CollisionMap
{
using Square = eColType;
using Layer = Square**;


    /*
    ** LineReference
    */
    class LineReference
    {
    public:
        LineReference(CollisionMap *map, uint16_t layerIdx, uint32_t lineIdx);

        Square&             operator[](unsigned int idx);

    private:
        CollisionMap*       _map;
        uint16_t            _layerIdx;
        uint32_t            _lineIdx;
    };


    /*
    ** LayerReference
    */
    class LayerReference {
    public:
        LayerReference(CollisionMap *map, unsigned int idx);

        LineReference       operator[](unsigned int idx);

    private:
        uint16_t            _layerIdx;
        CollisionMap*       _map;
    };


    /*
    ** CollisionMap
    */

    friend LineReference;
    friend LayerReference;

public:
    CollisionMap(EntityManager& em, Map* map);
    ~CollisionMap();

    LayerReference          operator[](unsigned int idx);
    eColType                colTypeFromEntityType(eEntityType entityType) const;

private:
    Map*                    _map;
    Layer*                  _collisionMap;
    EntityManager&          _em;
};
