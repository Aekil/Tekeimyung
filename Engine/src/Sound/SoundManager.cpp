
#include "Sound/SoundManager.hpp"

std::shared_ptr<SoundManager>   SoundManager::_soundManager = nullptr;

SoundManager::SoundManager() :
_result(FMOD_OK),
_system(nullptr),
_channel(nullptr),
_bkgdMusic(nullptr)
{}

SoundManager::~SoundManager()
{}

std::shared_ptr<SoundManager>   SoundManager::getInstance()
{
    if (!_soundManager)
    {
        _soundManager = std::make_shared<SoundManager>();
    }
    return _soundManager;
}

bool    SoundManager::initialize()
{
    _result = FMOD::System_Create(&_system);
    if (!errorCheck())
        return (false);

    _result = _system->init(SOUND_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (!errorCheck())
        return (false);

    return (true);
}

void    SoundManager::update()
{
    _result = _system->update();
    errorCheck();
    //ASSERT(errorCheck(), "FMOD Error");
}

void    SoundManager::shutdown()
{
    _result = _bkgdMusic->release();
    errorCheck();
    _result = _system->close();
    errorCheck();
    _result = _system->release();
    errorCheck();
}

bool    SoundManager::errorCheck()
{
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
        return (false);
    }
    return (true);
}

void    SoundManager::playBkgdMusic(const char *name, bool loop)
{
    FMOD_MODE mode;

    mode = (loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT);
    _result = _system->createStream(name, mode, 0, &_bkgdMusic);
    errorCheck();
    _result = _system->playSound(_bkgdMusic, 0, false, &_channel);
    errorCheck();
}

/*_result = _system->createSound("test.mp3", FMOD_DEFAULT, 0, &_bkgdMusic);
_result = _bkgdMusic->setMode(FMOD_LOOP_OFF);*/