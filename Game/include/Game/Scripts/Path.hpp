/*
** Author : Guillaume LABEY
*/

#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

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
    Path() = default;
    ~Path();

    std::vector<glm::ivec2>         goToTarget(glm::ivec2 pos, glm::ivec2 target, int map[29][38], const glm::ivec2& mapSize);

private:
    Node*                           getNodeFromPos(int x, int y, int map[29][38]);
    void                            freeNodes();
    void                            generateNodes(int map[29][38]);
    std::vector<Node*>              getAdjacentNodes(Node* fromNode);
    std::vector<Node*>              getAdjacentWalkableNodes(Node* fromNode);
    bool                            findTarget(Node* currentNode);


private:
    Node***                         _nodes = nullptr;
    glm::vec2                       _mapSize;

    glm::ivec2                      _from;
    glm::ivec2                      _target;
};
