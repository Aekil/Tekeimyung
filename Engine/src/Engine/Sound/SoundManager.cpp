#include <iostream>

#include <Engine/Utils/Debug.hpp>
#include <Engine/Utils/Logger.hpp>
#include <Engine/Utils/RessourceManager.hpp>

#include <Engine/Sound/SoundManager.hpp>

std::shared_ptr<SoundManager>   SoundManager::_soundManager = nullptr;

SoundManager::SoundManager() :
_result(FMOD_OK),
_system(nullptr),
_channel1(nullptr),
_channel2(nullptr)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        _sounds[i].free = true;
        _sounds[i].id = i;
        _sounds[i].name = nullptr;
        _sounds[i].sound = nullptr;
        _sounds[i].type = DEFAULT_SOUND;
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

    _result = _system->init(SOUND_MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
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

void    SoundManager::freeSound(unsigned int id)
{
    if (_sounds[id].free == false)
    {
        _sounds[id].free = true;
        _sounds[id].name = nullptr;
        _result = _sounds[id].sound->release();
        errorCheck();
    }
}

int     SoundManager::registerSound(const char *name, eSoundType type)
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (_sounds[i].free)
        {
            _sounds[i].free = false;
            _sounds[i].type = type;
            _sounds[i].name = name;
            if (type == BACKGROUND_SOUND)
            {
                _result = _system->createStream(name, FMOD_LOOP_NORMAL, 0, &_sounds[i].sound);
            }
            else // type == DEFAULT_SOUND
            {
                _result = _system->createSound(name, FMOD_DEFAULT, 0, &_sounds[i].sound);
            }
            errorCheck();
            return (_sounds[i].id);
        }
    }
    LOG_INFO("No place available to register more sounds"); // log : info / warn ?
    return (-1); // no place available to register more sounds
}

void    SoundManager::playSound(unsigned int id)
{
    if (_sounds[id].type == BACKGROUND_SOUND)
    {
        _result = _system->playSound(_sounds[id].sound, 0, false, &_channel1);
    }
    else
    {
        _result = _system->playSound(_sounds[id].sound, 0, false, &_channel2);
    }
    errorCheck();
}

void    SoundManager::addSoundStrings(std::string path, std::string name)
{
    tSoundStrings tmp = { path, name };

    _soundsStrings.push_back(tmp);
}

const std::vector<tSoundStrings>&  SoundManager::getSoundsStrings() const
{
    return (_soundsStrings);
}

const char*   SoundManager::getSoundNameFromID(int id) const
{
    for (int i = 0; i < NB_MAX_SOUNDS; i++)
    {
        if (i == id)
        {
            if (!_sounds[i].free)
            {
                return (_sounds[i].name);
            }
            else
            {
                break;
            }
        }
    }
    return (nullptr);
}