#include <algorithm>

#include <Game/Scripts/GameManager.hpp>
#include <Game/Scripts/Path.hpp>

Node::Node(glm::ivec2 pos_, bool isWalkable_)
{
    pos = pos_;
    isWalkable = isWalkable_;
    state = Node::NodeState::Untested;
    g = 0;
    h = 0;
}

float   Node::getDistance(const Node* to)
{
    int dstX = abs(this->pos.x - to->pos.x);
    int dstY = abs(this->pos.y - to->pos.y);

    if (dstX > dstY)
        return static_cast<float>(14 * dstY + 10 * (dstX - dstY));
    return static_cast<float>(14 * dstX + 10 * (dstY - dstX));
}

Path::~Path()
{
    freeNodes();
}

Node*    Path::getNodeFromPos(int x, int y, int map[29][38])
{
    if (x < 0 || x > _mapSize .x - 1 || y < 0 || y > _mapSize.y - 1)
        return nullptr;

    glm::ivec2 pos(x, y);
    bool isWalkable = (map[x][y] % LAYER_NUMBER) == 1;
    return (new Node(pos, isWalkable));
}

void    Path::freeNodes()
{
    if (!_nodes)
        return;

    for (int x = 0; x < _mapSize.x; ++x)
    {
        for (int y = 0; y < _mapSize.y; ++y)
        {
            delete _nodes[x][y];
        }
        delete _nodes[x];
    }

    delete _nodes;
}

void    Path::generateNodes(int map[29][38])
{
    freeNodes();
    _nodes = new Node**[(int)_mapSize.x];
    for (int x = 0; x < _mapSize.x; ++x)
    {
        _nodes[x] = new Node*[(int)_mapSize.y];
        for (int y = 0; y < _mapSize.y; ++y)
        {
            _nodes[x][y] = getNodeFromPos(x, y, map);
        }
    }
}

std::vector<Node*>   Path::getAdjacentNodes(Node* fromNode)
{
    std::vector<Node*> adjacentNodes;
    std::vector<glm::ivec2> locations{
        glm::ivec2(fromNode->pos.x, fromNode->pos.y + 1),
        glm::ivec2(fromNode->pos.x, fromNode->pos.y - 1),
        glm::ivec2(fromNode->pos.x + 1, fromNode->pos.y),
        glm::ivec2(fromNode->pos.x - 1, fromNode->pos.y),
    };

    for (auto& location: locations)
    {
        int x = location.x;
        int y = location.y;
        // Not out of range
        if (!(x < 0 || x > _mapSize.x - 1 || y < 0 || y > _mapSize.y - 1))
            adjacentNodes.push_back(_nodes[x][y]);
    }

    return (adjacentNodes);
}

std::vector<Node*>   Path::getAdjacentWalkableNodes(Node* fromNode)
{
    std::vector<Node*> walkableNodes;
    std::vector<Node*> nextLocations = getAdjacentNodes(fromNode);

    for (Node* node: nextLocations)
    {
        int x = node->pos.x;
        int y = node->pos.y;
        // Out of range
        if (x < 0 || x > _mapSize.x - 1 || y < 0 || y > _mapSize.y - 1)
            continue;

        // Non walkable node
        if (!node->isWalkable)
        {
            continue;
        }

        // Ignore already-closed nodes
        if (node->state == Node::NodeState::Closed)
            continue;

        // Already-open nodes are only added to the list if their G-value is lower going via this route.
        float newMovementCost = fromNode->g + fromNode->getDistance(node);
        if (node->state == Node::NodeState::Untested ||
            newMovementCost < node->g)
        {
            node->g = newMovementCost;
            node->h = node->getDistance(_nodes[_target.x][_target.y]);
            node->parent = fromNode;
            walkableNodes.push_back(node);
        }
    }

    return (walkableNodes);
}

bool sortNodes(const Node* lhs, const Node* rhs)
{
    return ((lhs->h + lhs->g) < (rhs->h + rhs->g));
}

bool    Path::findTarget(Node* currentNode)
{
    currentNode->state = Node::NodeState::Closed;
    std::vector<Node*> nextNodes = getAdjacentWalkableNodes(currentNode);

    std::sort(nextNodes.begin(), nextNodes.end(), sortNodes);
    for (Node* nextNode: nextNodes)
    {
        if (nextNode->pos.x == _target.x && nextNode->pos.y == _target.y)
        {
            return (true);
        }
        else
        {
            if (findTarget(nextNode)) // Note: Recurses back into Search(Node)
                return (true);
        }
    }
    return (false);
}

std::vector<glm::ivec2>  Path::goToTarget(glm::ivec2 pos, glm::ivec2 target, int map[29][38], const glm::ivec2& mapSize)
{
    std::vector<glm::ivec2> path;

    _mapSize = mapSize;

    _from = pos;
    _target = target;

    generateNodes(map);
    findTarget(_nodes[_from.x][_from.y]);

    Node* node = _nodes[_target.x][_target.y];
    while (node->parent != nullptr)
    {
        path.push_back(node->pos);
        node = node->parent;
    }

    std::reverse(path.begin(), path.end());
    return (path);
}
