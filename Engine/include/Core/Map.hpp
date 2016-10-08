#pragma once

#include <cstdint>
#include <cstddef>
#include <glm/vec3.hpp>
#include "Core/CollisionMap.hpp"
#include "EntityManager.hpp"

class CollisionMap;

class Map
{
using Square = struct {
    uint32_t static_;
    uint32_t dynamic_;
};
using Layer = Square**;

public:
    enum class eObjType
    {
        STATIC,
        DYNAMIC
    };

private:
    /*
    ** SquareReference
    */
    class SquareReference
    {
    public:
        SquareReference(Map *map, uint16_t layerIdx, uint32_t lineIdx, uint32_t squareIdx);

        void                set(eObjType pos, uint32_t id);
        uint32_t&           get(eObjType pos);

    private:
        Map*                _map;
        uint16_t            _layerIdx;
        uint32_t            _lineIdx;
        uint32_t            _squareIdx;
    };


    /*
    ** LineReference
    */
    class LineReference
    {
    public:
        LineReference(Map *map, uint16_t layerIdx, uint32_t lineIdx);

        SquareReference     operator[](unsigned int idx);

    private:
        Map*                _map;
        uint16_t            _layerIdx;
        uint32_t            _lineIdx;
    };


    /*
    ** LayerReference
    */
    class LayerReference {
    public:
        LayerReference(Map *map, unsigned int idx);

        LineReference       operator[](unsigned int idx);

    private:
        uint16_t            _layerIdx;
        Map*                _map;
    };


    /*
    ** Map
    */

    friend SquareReference;
    friend LineReference;
    friend LayerReference;

public:
    Map(EntityManager& em, uint32_t width, uint32_t height, uint16_t layersNb);
    ~Map();

    LayerReference          operator[](unsigned int idx);

    uint32_t                getWidth() const;
    uint32_t                getHeight() const;
    uint16_t                getLayersNb() const;
    CollisionMap*           getCollisionMap() const;
    bool                    isValidPosition(glm::ivec3& pos) const;

private:
    Layer*                  _map;
    CollisionMap*           _collisionMap;
    EntityManager&          _em;

    uint32_t                _layersNb;
    uint32_t                _width;
    uint32_t                _height;
};
