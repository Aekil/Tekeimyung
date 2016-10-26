
#include "Sound/SoundManager.hpp"

SoundManager::SoundManager() :
_result(FMOD_OK),
_system(nullptr)
{}

SoundManager::~SoundManager()
{}

int     SoundManager::initialize()
{
    std::cout << "test" << std::endl;
    _result = FMOD::System_Create(&_system);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " /*<< FMOD_ErrorString(_result)*/ << std::endl;
        return (1);
    }

    _result = _system->init(SOUND_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " /*<< FMOD_ErrorString(_result)*/ << std::endl;
        return (1);
    }

    //tmp test
    //playBkgdMusic("Kalimba.mp3", false);
    return (0);
}

void    SoundManager::update()
{
    _result = _system->update();
}

void    SoundManager::shutdown()
{
    _result = _system->release();
}

void    SoundManager::playBkgdMusic(const char *name, bool loop)
{
    FMOD_MODE mode;

    mode = (loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT);
   // _result = _system->createStream(name, mode, nullptr, _bkgdMusic);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " /*<< FMOD_ErrorString(_result)*/ << std::endl;
    }
}