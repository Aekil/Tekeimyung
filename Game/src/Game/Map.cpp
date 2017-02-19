/**
* @Author   Guillaume Labey
*/

#include <cstring>
#include <cmath>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Window/GameWindow.hpp>
#include <Engine/Graphics/Camera.hpp>
#include <Engine/Graphics/Renderer.hpp>
#include <Engine/Physics/Collisions.hpp>

#include <Engine/Components.hh>

#include <Game/Map.hpp>


/*
** SquareReference
*/

Map::SquareReference::SquareReference(Map *map, uint16_t layerIdx, uint32_t lineIdx, uint32_t squareIdx): _map(map), _layerIdx(layerIdx), _lineIdx(lineIdx), _squareIdx(squareIdx) {}

Map::Square& Map::SquareReference::operator=(uint32_t id)
{
    _map->_map[_layerIdx][_lineIdx][_squareIdx] = id;
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

void    Map::LayerReference::addEntity(uint32_t entity)
{
    _map->_entities[_layerIdx].push_back(entity);
}

void    Map::LayerReference::orderEntities(EntityManager& em)
{
    auto it = _map->_entities[_layerIdx].cbegin();
    while (it != _map->_entities[_layerIdx].cend())
    {
        // The emitter has been deleted, remove it from the map
        if (!em.getEntity(*it))
        {
            // Remove emitter from map
            _map->_entities[_layerIdx].erase(it++);
        }
        else
            ++it;
    }

    _map->_entities[_layerIdx].sort([&](uint32_t a, uint32_t b) {
        /*Entity* entityA = em.getEntity(a);
        Entity* entityB = em.getEntity(b);
        sPositionComponent* positionA;
        sPositionComponent* positionB;
        float dirty;


        ASSERT(entityA && entityB, "Entities should exists");

        positionA = entityA->getComponent<sPositionComponent>();
        positionB = entityB->getComponent<sPositionComponent>();

        float posA = (std::floor(positionA->value.y) * _map->_width) + std::floor(positionA->value.x) + (std::pow(std::modf(positionA->value.x, &dirty), 2) + std::pow(std::modf(positionA->value.y, &dirty), 2)) / 2.0f;
        float posB = (std::floor(positionB->value.y) * _map->_width) + std::floor(positionB->value.x) + (std::pow(std::modf(positionB->value.x, &dirty), 2) + std::pow(std::modf(positionB->value.y, &dirty), 2)) / 2.0f;
        return posA < posB;*/
        return true;
    });
}

std::list<uint32_t>& Map::LayerReference::getEntities()
{
    return _map->_entities[_layerIdx];
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

bool    Map::isValidPosition(const glm::ivec3& pos) const
{
    return (pos.x >= 0 && pos.x < (int)getWidth() &&
        pos.y >= 0 && pos.y < (int)getHeight() &&
        pos.z >= 0 && pos.z < (int)getLayersNb());
}

glm::vec3   Map::mapToGraphPosition(const glm::vec2& mapPos, float z)
{
    glm::vec3 graphPos;
    float tileWidthHalf = 25.0f;
    float tileLengthHalf = 25.0f;
    float tileHeight = 12.5f;

    graphPos.x = mapPos.x * tileLengthHalf;
    graphPos.y = tileHeight * z;
    graphPos.z = mapPos.y * tileWidthHalf;

    return (graphPos);
}

Entity* Map::getSelectedEntity(EntityManager* em, bool onlyTiles)
{
    auto gameWindow = GameWindow::getInstance();
    Mouse& mouse =  gameWindow->getMouse();
    Cursor& cursor = mouse.getCursor();

    glm::vec3 nearScreen(cursor.getX(), gameWindow->getBufferHeight() - cursor.getY(), 0.0f);
    glm::vec3 farScreen(cursor.getX(), gameWindow->getBufferHeight() - cursor.getY(), 1.0f);

    Camera* camera = Renderer::getInstance()->getCurrentCamera();

    if (!camera)
        return (nullptr);

    // Unproject 2D points to get 3D points
    // Get 3D point on near plane
    glm::vec3 nearPoint = glm::unProject(nearScreen, camera->getView(), camera->getProj(), gameWindow->getViewport());
    // Get 3D point on far plane
    glm::vec3 farPoint = glm::unProject(farScreen, camera->getView(), camera->getProj(), gameWindow->getViewport());

    Entity* selectedEntity = nullptr;
    float selectedEntityDist = 0.0f;

    for (Entity* entity : em->getEntities())
    {
        sRenderComponent* render = entity->getComponent<sRenderComponent>();
        sTransformComponent* transform = entity->getComponent<sTransformComponent>();

        // We can't select entity that is not displayed
        if (!render)
            continue;

        // The selectable zone of an entity is a box
        glm::vec3 boxPos;
        glm::vec3 boxCenter;
        glm::vec3 boxSize;

        // Model box collider position
        boxPos = glm::vec3(render->getModel()->getMin().x, render->getModel()->getMin().y, render->getModel()->getMin().z);

        // Model box collider size
        boxSize.x = render->getModel()->getSize().x;
        boxSize.y = render->getModel()->getSize().y;
        boxSize.z = render->getModel()->getSize().z;

        // Model box collider center
        boxCenter = boxPos;
        boxCenter.x += (boxSize.x / 2.0f);
        boxCenter.y += (boxSize.y / 2.0f);
        boxCenter.z += (boxSize.z / 2.0f);

        // Convert box collider pos and center to world position
        boxPos += transform->pos;
        boxCenter += transform->pos;

        // Calculate vector from near plane to far plane
        glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

        // Calculate the length of this vector
        float rayLength = glm::distance(nearPoint, farPoint);

        // Calculate vector from near plane to object
        glm::vec3 objDir = boxCenter - nearPoint;

        // Project objDir onto rayDir vector
        // It gives us a scalar that is the length of objDir onto rayDir
        float scalar = glm::dot(objDir, rayDir);


        // Find closest point
        glm::vec3 closestPoint;

        // The projected objDir length is less or equal to 0 (the projected vector objDir points backward)
        // (So the point is the nearPoint or before the nearPoint)
        if (scalar <= 0.0f)
            closestPoint = nearPoint;
        // The projected objDir length is greater or equal than the vector that goes from near plane to far plane
        // (So the point is the farPoint or behind the farPoint)
        else if (scalar >= rayLength)
            closestPoint = farPoint;
        // The point is between the near plane and the far plane
        else
            closestPoint = nearPoint + (rayDir * scalar);

        // Check if the point is in the box of the entity selection
        if (Collisions::pointVSAABB(closestPoint, boxPos, boxSize))
        {
            // Calculate point distance
            float closestPointDist = glm::distance(nearPoint, closestPoint);

            // The ray can select multiple entities, we need to store the closest one
            if (selectedEntity == nullptr || closestPointDist <= selectedEntityDist)
            {
                selectedEntityDist = closestPointDist;
                selectedEntity = entity;
            }
        }
    }

    return (selectedEntity);
}
