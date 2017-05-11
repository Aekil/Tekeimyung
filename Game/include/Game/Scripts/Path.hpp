/*
** Author : Guillaume LABEY
*/

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include <Game/DoubleArray.hpp>
#include <Game/Map.hpp>

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

    std::vector<glm::ivec2>         goToTarget(glm::ivec2 pos, glm::ivec2 target, Map* map);

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

    Map*                            _map = nullptr;

    std::vector<Node*>              _openNodes;
};
