/*
** Author : Guillaume LABEY
*/

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include <Game/Map/DoubleArray.hpp>
#include <Game/Map/Map.hpp>

class Node
{
public:
    enum class NodeState
    {
        Untested = 0,
        Open = 1,
        Closed = 2
    };

public:
    Node() = default;
    Node(glm::ivec2 pos_, bool isWalkable_);
    ~Node() = default;

    float       getDistance(const Node* to);

public:
    NodeState   state = NodeState::Untested;
    glm::ivec2  pos;
    float       g = 0;
    float       h = 0;
    bool        isWalkable = false;
    Node*       parent = nullptr;
};

class Path
{
public:
    Path();
    ~Path();

    bool goToTarget(glm::ivec2 pos, glm::ivec2 target, Map* map, std::vector<glm::ivec2>& path, glm::ivec2& closestTileFound);
    bool isWalkable(Map& map, int x, int y);

private:
    Node*                           getNodeFromPos(int x, int y);
    void                            freeNodes();
    void                            generateNodes();
    std::vector<Node*>              getAdjacentNodes(Node* fromNode);
    std::vector<Node*>              getAdjacentWalkableNodes(Node* fromNode);
    bool                            findTarget();

    bool                            isOutOfRange(int x, int y) const;


private:
    DoubleArray<Node*>              _nodes;

    glm::ivec2                      _from;
    glm::ivec2                      _target;
    glm::ivec2*                     _closestTileFound = nullptr;

    Map*                            _map = nullptr;

    std::vector<Node*>              _openNodes;
};
