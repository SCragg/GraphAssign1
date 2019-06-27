/*SimpleAudioManager.h
Basic functionality taken from 'Getting started with C++ audio programming for game development" (David Gouveia 2013)

I added a few extra functions for this project.

Can Load an audio file and play the file.

*/
#pragma once

#include <string>
#include <map>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

typedef std::map<std::string, FMOD::Sound*> SoundMap;

class SimpleAudioManager
{
public:
	SimpleAudioManager();
	~SimpleAudioManager();

	//Update must be called in the mainloop
	void Update(float elapsed);
	//Load a sound/stream from disk
	void Load(const std::string& path);
	//Play an audio stream
	void Stream(const std::string& path);
	//Play a loaded sound
	void Play(const std::string& path);

	//These functions are used to control speed and pause and unpause the audio which I added myself
	//Pause the audio 
	void Pause();
	//Unpause the audio
	void Unpause();
	//Returns true to isplaying if audio is playing
	void IsAudioPlaying(bool* isplaying);
	//Returns true if audio is currently paused
	void IsAudioPaused(bool* ispaused);
	//Sets pitch(/speed) of audio
	void SetPitch(float pitch);
	//Set position of audio
	void SetPosition(float position);

private:
	void LoadOrStream(const std::string& path, bool stream);
	FMOD::System* system;
	FMOD::Channel* channel;
	SoundMap sounds;

	unsigned int audiolength;
};