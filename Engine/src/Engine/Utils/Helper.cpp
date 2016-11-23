#include <cstdlib>
#include <ctime>

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
