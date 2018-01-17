#include <audio_source.h>
#include <iostream>

AudioSource::AudioSource(AudioBuffer* buffer) {
	
	alGenSources(1, &sourceId);	
	bufferId = buffer->getAlBuffer();
	alSourcei(sourceId, AL_BUFFER, bufferId);
	pitch = 1;
	gain = 1;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	vel.x = 0;
	vel.y = 0;
	vel.z = 0;

	alSource3f(sourceId, AL_POSITION, pos.x, pos.y, pos.z);
	alSource3f(sourceId, AL_VELOCITY, vel.x, vel.y, vel.z);

}

bool AudioSource::isPlaying() {

	alGetSourcei(sourceId, AL_SOURCE_STATE, &playing); 
	
	if (playing == AL_PLAYING)
		return true;
	else
		return false;

}

void AudioSource::setLooping(bool loop) {
	if (loop)
		alSourcei(sourceId, AL_LOOPING, AL_TRUE);
	else
		alSourcei(sourceId, AL_LOOPING, AL_FALSE);

}

void AudioSource::addPitch(float addPitchFactor) {
	
	pitch += addPitchFactor;
	alSourcef(sourceId, AL_PITCH, pitch);

}

void AudioSource::setPosition(float x, float y, float z) {
	alSource3f(sourceId, AL_POSITION, pos.x, pos.y, pos.z);

}
void AudioSource::setVelocity(float x, float y, float z) {
	alSource3f(sourceId, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioSource::movePositionX(float ratio) {
	pos.x += ratio;
	alSource3f(sourceId, AL_POSITION, pos.x, pos.y, pos.z);
	
}

AudioSource::~AudioSource() {
	alDeleteSources(1, &sourceId);
}