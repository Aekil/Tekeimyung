#include <cmath>

#include <Engine/Physics/Collisions.hpp>

Collisions::Collisions()
{}

Collisions::~Collisions()
{}

bool    Collisions::rectHitboxCheck(tRectHitbox *hb1, tRectHitbox *hb2)
{
    if ((hb1->max.x < hb2->min.x || hb1->min.x > hb2->max.x) ||
        (hb1->max.y < hb2->min.y || hb1->min.y > hb2->max.y))
        return (false);
    return (true);
}

bool    Collisions::rectHitboxCheck(glm::vec2 *min1, glm::vec2 *max1, glm::vec2 *min2, glm::vec2 *max2)
{
    if ((max1->x < min2->x || min1->x > max2->x) ||
        (max1->y < min2->y || min1->y > max2->y))
        return (false);
    return (true);
}

bool    Collisions::circleHitboxCheck(tCircleHitbox *hb1, tCircleHitbox *hb2)
{
    float radius;
    float result;

    radius = pow((hb1->radius + hb2->radius), 2);
    result = pow((hb1->center.x + hb2->center.x), 2) + pow((hb1->center.y + hb2->center.y), 2);
    if (radius < result)
        return (true);
    return (false);
}

bool    Collisions::circleHitboxCheck(glm::vec2 *center1, float radius1, glm::vec2 *center2, float radius2)
{
    float radius;
    float result;
    
    radius = pow((radius1 + radius2), 2);
    result = pow((center1->x + center2->x), 2) + pow((center1->y + center2->y), 2);
    if (radius < result)
        return (true);
    return (false);
}