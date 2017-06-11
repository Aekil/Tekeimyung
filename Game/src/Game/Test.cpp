
#include <Game/Scripts/Test.hpp>

REGISTER_SCRIPT(Test);

void Test::start()
{
    for (uint32_t i = 0; i < 7; ++i)
    {
        for (uint32_t j = 0; j < 7; ++j)
        {
            this->Instantiate("FIRE_BALL", glm::vec3(i * 25, 0, j * 25));
        }
    }
}

void Test::update(float dt)
{
}
