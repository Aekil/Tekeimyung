#include <cstring>
#include "Core/Map.hpp"


/*
** SquareReference
*/

Map::SquareReference::SquareReference(Map *map, uint16_t layerIdx, uint32_t lineIdx, uint32_t squareIdx): _map(map), _layerIdx(layerIdx), _lineIdx(lineIdx), _squareIdx(squareIdx) {}

Map::Square&    Map::SquareReference::operator=(uint32_t id)
{
    //TODO: Add assert to verify that layerIdx, lineIdx and squareIdx are not out of range

    _map->_map[_layerIdx][_lineIdx][_squareIdx] = id;
    (*_map->_collisionMap)[_layerIdx][_lineIdx][_squareIdx] = eColType::DIRTY;
    return get();
}

Map::Square&    Map::SquareReference::get()
{
    return _map->_map[_layerIdx][_lineIdx][_squareIdx];
}



/*
** LineReference
*/

Map::LineReference::LineReference(Map *map, uint16_t layerIdx, uint32_t lineIdx): _map(map), _layerIdx(layerIdx), _lineIdx(lineIdx) {}

Map::SquareReference    Map::LineReference::operator[](unsigned int idx)
{
    return SquareReference(_map, _layerIdx, _lineIdx, idx);
}



/*
** LayerReference
*/

Map::LayerReference::LayerReference(Map *map, unsigned int idx): _map(map), _layerIdx(idx) {}

Map::LineReference  Map::LayerReference::operator[](unsigned int idx)
{
    return LineReference(_map, _layerIdx, idx);
}



/*
** Map
*/

Map::Map(EntityManager& em, uint32_t width, uint32_t height, uint16_t layersNb):
    _width(width), _height(height), _layersNb(layersNb), _em(em)
{
    _map = new Map::Layer[_layersNb];
    _collisionMap = new CollisionMap(_em, this);

    for (uint16_t layer = 0; layer < _layersNb; layer++)
    {
        _map[layer] = new Map::Square*[_height];
        for (uint32_t y = 0; y < _height; y++)
        {
            _map[layer][y] = new Map::Square[_width];

            std::memset(_map[layer][y], 0, sizeof(_map[layer][y][0]) * _width);
        }
    }
}

Map::~Map()
{
    for (uint16_t layer = 0; layer < _layersNb; layer++)
    {
         for (uint32_t y = 0; y < _height; y++)
        {
            delete _map[layer][y];
        }
        delete _map[layer];
    }
    delete _map;
}

Map::LayerReference Map::operator[](unsigned int idx)
{
    return LayerReference(this, idx);
}

uint32_t    Map::getWidth() const
{
    return (_width);
}

uint32_t    Map::getHeight() const
{
    return (_height);
}

uint16_t    Map::getLayersNb() const
{
    return (_layersNb);
}

CollisionMap*   Map::getCollisionMap() const
{
    return (_collisionMap);
}

bool    Map::isValidPosition(glm::ivec3& pos) const
{
    return (pos.x >= 0 && pos.x < (int)getWidth() &&
        pos.y >= 0 && pos.y < (int)getHeight() &&
        pos.z >= 0 && pos.z < (int)getLayersNb());
}
