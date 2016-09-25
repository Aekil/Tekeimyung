#include "Core/Engine.hpp"

int 	main()
{
    Engine engine;

    if (!engine.init() || !engine.run())
        return (1);

    engine.stop();
    return (0);
}