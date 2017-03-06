/**
* @Author   Guillaume Labey
*/

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>

#include <Engine/Utils/Debug.hpp>

#include <Engine/Utils/Helper.hpp>

Helper::Helper()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
}

Helper::~Helper() {}

float   Helper::randFloat(float from, float to)
{
    float max = from > to ? from : to;
    float min = from < to ? from : to;

    if (max - min == 0)
        return (0.0f);

    return (min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))));
}

int Helper::randInt(int from, int to)
{
    int max = from > to ? from : to;
    int min = from < to ? from : to;

    if (max - min == 0)
        return (0);

    return (min + (rand() / (RAND_MAX / (max - min))));
}

std::string Helper::lowerCaseString(const std::string& str)
{
    std::string lowerCase = str;
    std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);

    return (lowerCase);
}

float   Helper::parametricBlend(float t)
{
    float square = t * t;
    return (square / (2.0f * (square - t) + 1.0f));
}

float   Helper::smoothStep(float t)
{
    return (t * t * (3.0f - 2.0f * t));
}

float   Helper::smootherStep(float t)
{
    return (t * t * t * (t * (6.0f * t - 15.0f) + 10.0f));
}

bool    Helper::updateComboString(const char* name, std::vector<const char*>& stringList, std::string& stringValue)
{
    // Get index of string in the vector
    int stringIdx = -1;
    uint32_t i = 0;
    for (const auto& str: stringList)
    {
        if (str == stringValue)
        {
            stringIdx = i;
            break;
        }
        ++i;
    }

    // Display combo and set new value
    if (ImGui::Combo(name, &stringIdx, stringList.data(), (uint32_t)stringList.size()))
    {
        stringValue = stringList[stringIdx];
        return (true);
    }
    return (false);
}
