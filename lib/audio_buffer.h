#pragma once
#include <al.h>
#include <alc.h>
#include <conio.h>
#include <vector>
class AudioBuffer {
private:
	AudioBuffer() {}
	ALuint* bufferId;

public:
	static AudioBuffer*	load(const char* filename);
	uint32_t			getAlBuffer() const {}
	~AudioBuffer() {}
};