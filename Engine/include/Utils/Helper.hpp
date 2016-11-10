#pragma once

#include <assimp/matrix4x4.h>
#include <glm/mat4x4.hpp>

class Helper
{
public:
    Helper();
    ~Helper();
    static float        randFloat(float from, float to);
    static int          randInt(int from, int to);
    static void         copyAssimpMat(const aiMatrix4x4& from, glm::mat4& to);
};
