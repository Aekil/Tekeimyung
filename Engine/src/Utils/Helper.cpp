#include <cstdlib>
#include <ctime>

#include "Utils/Debug.hpp"
#include "Utils/Helper.hpp"

Helper::Helper()
{
    std::srand(std::time(0));
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
