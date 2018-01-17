#pragma once
#include <al.h>
#include <alc.h>
#include <conio.h>
#include <vector>
#include <audio_buffer.h>

struct vec3 {
	float x;
	float y;
	float z;

};

class AudioSource {
private:
	uint32_t sourceId;
	uint32_t bufferId;
	ALint playing;
	float pitch;
	ALint gain;
	bool loop;
	vec3 pos;
	vec3 vel;

public:
	AudioSource(AudioBuffer* buffer);
	~AudioSource();
	void setPitch(float pitch) { alSourcei(sourceId, AL_PITCH, pitch); }
	void addPitch(float pichAdd);
	void setGain(float gain) { alSourcei(sourceId, AL_GAIN, gain); }
	void setLooping(bool loop);
	void setPosition(float x, float y, float z);
	void movePositionX(float ratio);
	void setVelocity(float x, float y, float z);
	void play() { alSourcePlay(sourceId); playing = true; }
	void stop() { alSourceStop(sourceId); playing = false;}
	void pause() { alSourcePause(sourceId); playing = false;}
	bool isPlaying();
	uint32_t			getAlSource() const { return sourceId; }


};