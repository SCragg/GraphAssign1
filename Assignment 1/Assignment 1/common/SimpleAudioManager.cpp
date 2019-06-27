/*SimpleAudioManager.cpp
Taken from 'Getting started with C++ audio programming for game development" (David Gouveia 2013)

Can Load an audio file and play the file.
*/

#include "SimpleAudioManager.h"
#include <iostream>

SimpleAudioManager::SimpleAudioManager() //(David Gouveia 2013)
{
	FMOD::System_Create(&system);
	system->init(100, FMOD_INIT_NORMAL, 0);
}

SimpleAudioManager::~SimpleAudioManager() //(David Gouveia 2013)
{ 
	// Release every sound object and clear the map
	SoundMap::iterator iter;
	for (iter = sounds.begin(); iter != sounds.end(); ++iter) 
		iter->second->release();
	sounds.clear();
	// Release the system object
	system->release();
	system = 0;
}

void SimpleAudioManager::Update(float elapsed) //(David Gouveia 2013)
{
system->update();
}

void SimpleAudioManager::Load(const std::string& path) //(David Gouveia 2013)
{
	LoadOrStream(path, false);
}

void SimpleAudioManager::Stream(const std::string& path) //(David Gouveia 2013)
{ 
	LoadOrStream(path, true); 
}

//Taken from (David Gouveia 2013)
void SimpleAudioManager::Play(const std::string& path)
{ // Search for a matching sound in the map
	SoundMap::iterator sound = sounds.find(path); 
	// Ignore call if no sound was found 
	if (sound == sounds.end()) 
		return; 
	// Otherwise play the sound 
	system->playSound(sound->second, 0, false, &channel);
}

//Pause the audio 
void SimpleAudioManager::Pause()
{
	channel->setPaused(true);
}

//Unpause Audio
void SimpleAudioManager::Unpause()
{
	channel->setPaused(false);
}

//Finds out whether audio is paused or not
void SimpleAudioManager::IsAudioPaused(bool* paused)
{
	channel->getPaused(paused);
}

//Returns true to isplaying if audio is playing
void SimpleAudioManager::IsAudioPlaying(bool* isplaying)
{
	channel->isPlaying(isplaying);
}

//Sets pitch with a float value
void SimpleAudioManager::SetPitch(float pitch)
{
	channel->setPitch(pitch);
}

//Sets postion to position as a fraction of whole. eg 0.1, sets the audio position to 0.1*length of the song
void SimpleAudioManager::SetPosition(float position)
{
	unsigned int timeinms = position * audiolength;
	channel->setPosition(timeinms, FMOD_TIMEUNIT_MS);
}

void SimpleAudioManager::LoadOrStream(const std::string& path, bool stream) 
{
	FMOD_RESULT result;
	// Ignore call if sound is already loaded
	if (sounds.find(path) != sounds.end()) 
		return; 
	// Load (or stream) file into a sound object 
	FMOD::Sound* sound;
	if (stream)
	{
		result = system->createStream(path.c_str(), FMOD_DEFAULT, 0, &sound);
		if (result != FMOD_OK)
			std::cout << "Create stream is not OK: " << FMOD_ErrorString(result) << std::endl << std::endl;
	}
	else 
		system->createSound(path.c_str(), FMOD_DEFAULT, 0, &sound);
	// Store the sound object in the map using the path as key 
	sounds.insert(std::make_pair(path, sound));

	//Added this to get length of sound in ms which I use to work out position of the audio based on arm rotation
	sound->getLength(&audiolength, FMOD_TIMEUNIT_MS);
}

	