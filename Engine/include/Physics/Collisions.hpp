#pragma once

#include <glm/vec2.hpp>

typedef struct sRectHitbox
{
    glm::vec2   min;
    glm::vec2   max;
}               tRectHitbox;

typedef struct sCircleHitbox
{
    glm::vec2   center;
    float       radius;
}               tCircleHitbox;

class Collisions
{
public:
    Collisions();
    virtual ~Collisions();

    static bool         rectHitboxCheck(tRectHitbox *hb1, tRectHitbox *hb2);
    static bool         rectHitboxCheck(glm::vec2 *min1, glm::vec2 *max1, glm::vec2 *min2, glm::vec2 *max2);

    static bool         circleHitboxCheck(tCircleHitbox *hb1, tCircleHitbox *hb2);
    static bool         circleHitboxCheck(glm::vec2 *center1, float radius1, glm::vec2 *center2, float radius2);

private:

};