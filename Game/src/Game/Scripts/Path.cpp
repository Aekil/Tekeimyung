#include <algorithm>

#include <Game/Manager/GameManager.hpp>
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

    return sqrt((dstX * dstX) + (dstY * dstY));
}

Path::Path() {}

Path::~Path()
{
    freeNodes();
}

Node*    Path::getNodeFromPos(int x, int y)
{
    if (isOutOfRange(x, y))
        return nullptr;

    glm::ivec2 pos(x, y);
    return (new Node(pos, _map->isWalkable(x, y)));
}

void    Path::freeNodes()
{
    if (!_nodes.isAllocated())
    {
        return;
    }

    for (int x = 0; x < _nodes.getWidth(); ++x)
    {
        for (int y = 0; y < _nodes.getHeight(); ++y)
        {
            delete _nodes[x][y];
        }
    }
    _nodes.free();
}

void    Path::generateNodes()
{
    freeNodes();
    _nodes.allocate(_map->getWidth(), _map->getHeight());
    for (int x = 0; x < _map->getWidth(); ++x)
    {
        for (int y = 0; y < _map->getHeight(); ++y)
        {
            _nodes[x][y] = getNodeFromPos(x, y);
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
        if (!isOutOfRange(x, y))
            adjacentNodes.push_back(_nodes[x][y]);
    }

    return (adjacentNodes);
}

bool sortNodes(const Node* lhs, const Node* rhs)
{
    return ((lhs->h + lhs->g) < (rhs->h + rhs->g));
}

std::vector<Node*>   Path::getAdjacentWalkableNodes(Node* fromNode)
{
    std::vector<Node*> walkableNodes;
    std::vector<Node*> nextLocations = getAdjacentNodes(fromNode);

    for (Node* node: nextLocations)
    {
        int x = node->pos.x;
        int y = node->pos.y;

        // Non walkable node
        if (!node->isWalkable)
        {
            continue;
        }

        // Ignore already-closed nodes
        if (node->state == Node::NodeState::Closed)
            continue;

        float newMovementCost = fromNode->g + 1;
        node->g = newMovementCost;
        node->h = node->getDistance(_nodes[_target.x][_target.y]);
        node->parent = fromNode;
        walkableNodes.push_back(node);
    }

    return (walkableNodes);
}

bool    Path::findTarget()
{
    if (_openNodes.size() == 0)
        return (false);
    else
    {
        Node* closestNode = _nodes[_closestTileFound->x][_closestTileFound->y];
        Node* currentNode = _openNodes[0];
        if (closestNode->pos == _from ||
            currentNode->h < closestNode->h)
        {
            *_closestTileFound = currentNode->pos;
        }
    }

    Node* currentNode = _openNodes[0];
    _openNodes.erase(_openNodes.begin());

    currentNode->state = Node::NodeState::Closed;
    std::vector<Node*> nextNodes = getAdjacentWalkableNodes(currentNode);

    for (Node* nextNode: nextNodes)
    {
        if (nextNode->pos.x == _target.x && nextNode->pos.y == _target.y)
        {
            return (true);
        }

        if (std::find(_openNodes.begin(), _openNodes.end(), nextNode) == _openNodes.end())
        {
            _openNodes.push_back(nextNode);
        }

    }

    std::sort(_openNodes.begin(), _openNodes.end(), sortNodes);
    findTarget();
    return (false);
}

bool Path::goToTarget(glm::ivec2 pos, glm::ivec2 target, Map* map, std::vector<glm::ivec2>& path, glm::ivec2& closestTileFound)
{
    _map = map;

    if (isOutOfRange(pos.x, pos.y) ||
        isOutOfRange(target.x, target.y))
    {
        return (false);
    }

    _openNodes.clear();

    _from = pos;
    _target = target;
    _closestTileFound = &closestTileFound;
    *_closestTileFound = _from;

    generateNodes();
    _openNodes.push_back(_nodes[_from.x][_from.y]);
    findTarget();

    Node* node = _nodes[_target.x][_target.y];
    while (node->parent != nullptr)
    {
        path.push_back(node->pos);
        node = node->parent;
    }

    std::reverse(path.begin(), path.end());
    return (path.size() != 0);
}

bool    Path::isOutOfRange(int x, int y) const
{
    return (!_map ||
        x < 0 || y < 0 ||
        x >= _map->getWidth() || y >= _map->getHeight());
}
