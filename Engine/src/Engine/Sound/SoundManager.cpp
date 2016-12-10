#include <iostream>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Sound/SoundManager.hpp>

std::shared_ptr<SoundManager>   SoundManager::_soundManager = nullptr;

SoundManager::SoundManager() :
_result(FMOD_OK),
_system(nullptr)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        _sounds[i].free = true;
        _sounds[i].id = i;
        _sounds[i].sound = nullptr;
        _sounds[i].channel = nullptr;
        _sounds[i].type = eSoundType::DEFAULT_SOUND;
    }
}

SoundManager::~SoundManager() {}

std::shared_ptr<SoundManager>   SoundManager::getInstance()
{
    if (!_soundManager)
    {
        _soundManager = std::make_shared<SoundManager>();
    }
    return (_soundManager);
}

bool    SoundManager::initialize()
{
    _result = FMOD::System_Create(&_system);
    if (!errorCheck())
        return (false);

    _result = _system->init(NB_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
    if (!errorCheck())
        return (false);

    //RessourceManager::getInstance()->loadResources(SOUNDS_DIRECTORY);

//#if (ENGINE_DEBUG) // let similar message in release ?
    //for (int i = 0; i < _soundsStrings.size(); ++i)
    //{
    //    LOG_INFO("Sound found : path = %s"/* | basename = %s"*/, _soundsStrings[i].name.c_str()/*, _soundsStrings[i].name.c_str()*/);
    //}
//#endif

    return (true);
}

void    SoundManager::update()
{
    _result = _system->update();
    errorCheck();
}

void    SoundManager::shutdown()
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
        freeSound(i);
    _result = _system->close();
    errorCheck();
    _result = _system->release();
    errorCheck();
}

bool    SoundManager::errorCheck()
{
    if (_result != FMOD_OK)
    {
        LOG_ERROR("FMOD error! (%d) : %s", _result, FMOD_ErrorString(_result));
        //std::cerr << "FMOD error! (" << _result << ") " << FMOD_ErrorString(_result) << std::endl;
        return (false);
    }
    return (true);
}

void    SoundManager::freeSound(uint32_t id)
{
    if (_sounds[id].free == false)
    {
        _sounds[id].free = true;
        _result = _sounds[id].sound->release();
        errorCheck();
    }
}

int     SoundManager::registerSound(const std::string& name, eSoundType type)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        // The sound type changed, reload it to optimize
        if (_sounds[i].name == name &&
            _sounds[i].type != type &&
            _sounds[i].type != eSoundType::NONE)
        {
            _sounds[i].sound->release();
            _sounds[i].sound = nullptr;
            _sounds[i].free = true;
        }

        if (_sounds[i].free)
        {
            _sounds[i].free = false;
            _sounds[i].type = type;
            _sounds[i].name = name;
            _sounds[i].id = i;
            // Load music and default sound as streams
            if (type == eSoundType::BACKGROUND_SOUND || type == eSoundType::NONE)
            {
                _sounds[i].type = eSoundType::BACKGROUND_SOUND;
                _result = _system->createStream(name.c_str(), FMOD_LOOP_NORMAL, 0, &_sounds[i].sound);
            }
            else // type == DEFAULT_SOUND
            {
                _result = _system->createSound(name.c_str(), FMOD_DEFAULT, 0, &_sounds[i].sound);
            }
            errorCheck();
            return (_sounds[i].id);
        }
        else if (_sounds[i].name == name)
            return (_sounds[i].id);
    }
    LOG_INFO("No place available to register more sounds"); // log : info / warn ?
    return (-1); // no place available to register more sounds
}

void    SoundManager::playSound(uint32_t id)
{
    // Out of range
    if (id < 0 || id >= NB_MAX_SOUNDS)
        return;

    _result = _system->playSound(_sounds[id].sound, 0, false, &_sounds[id].channel);
    errorCheck();
}

void    SoundManager::stopSound(uint32_t id)
{
    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return;

    _result = _sounds[id].channel->stop();
    errorCheck();
}

bool    SoundManager::isSoundPlaying(uint32_t id)
{
    bool isPlaying = false;

    if (id < 0 || id >= NB_MAX_SOUNDS || // Out of range
        _sounds[id].channel == nullptr) // Not currently playing
        return (false);

    _sounds[id].channel->isPlaying(&isPlaying);
    errorCheck();

    return (isPlaying);
}

const char*   SoundManager::getSoundNameFromID(int id) const
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (i == id)
        {
            if (!_sounds[i].free)
            {
                return (_sounds[i].name.c_str());
            }
            else
            {
                break;
            }
        }
    }
    return (nullptr);
}
