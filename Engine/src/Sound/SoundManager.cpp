
#include "Sound/SoundManager.hpp"

std::shared_ptr<SoundManager>   SoundManager::_soundManager = nullptr;

SoundManager::SoundManager() :
_result(FMOD_OK),
_channel(nullptr),
_system(nullptr)
{}

SoundManager::~SoundManager()
{}

std::shared_ptr<SoundManager>   SoundManager::getInstance()
{
    if (!_soundManager)
    {
        _soundManager = std::make_shared<SoundManager>();
        //_soundManager = new SoundManager();
    }

    return _soundManager;
}

bool     SoundManager::initialize()
{
    _result = FMOD::System_Create(&_system);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
        return (false);
    }
    _result = _system->init(SOUND_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
        return (false);
    }

    /*FMOD::System_Create(&_system);
    _system->init(32, FMOD_INIT_NORMAL, 0);*/
    /*_system->createStream("test.mp3", FMOD_DEFAULT, 0, &_bkgdMusic);
    _system->playSound(_bkgdMusic, 0, false, &_channel);*/
    //playBkgdMusic("test.mp3", false);

    return (true);
}

void    SoundManager::update()
{
    _result = _system->update();
}

void    SoundManager::shutdown()
{
    _result = _bkgdMusic->release();
    _result = _system->close();
    _result = _system->release();
}

void    SoundManager::playBkgdMusic(const char *name, bool loop)
{
    FMOD_MODE mode;

    mode = (loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT);
    _result = _system->createStream(name, mode, 0, &_bkgdMusic);
    //_result = _system->createSound("test.mp3", FMOD_DEFAULT, 0, &_bkgdMusic);
    //_result = _bkgdMusic->setMode(FMOD_LOOP_OFF);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
    }
    _result = _system->playSound(_bkgdMusic, 0, false, &_channel);
    if (_result != FMOD_OK)
    {
        std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
    }
}