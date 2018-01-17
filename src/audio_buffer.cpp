#include <audio_buffer.h>
#include <iostream>


AudioBuffer*  AudioBuffer::load(const char* filename) {
	AudioBuffer *resultAudioBuffer = nullptr;
	FILE *ptr;

	
	ptr = fopen(filename, "rb");
	if (ptr == NULL) {
		return nullptr;
	}
	else {
		
		fseek(ptr, 0, SEEK_SET);
		long fmtChunkSize;
		long sampleRate;
		long dataSize;
		short bitsPerSample;
		short channels;
		short extraParamSize;
		size_t bufferSize = 4;
		size_t currentChar = 16;
		bool foundData = false;
		uint32_t bufferId = 0;
		
		std::vector<unsigned char> charBuffer(bufferSize);
		std::vector<unsigned char> dataBuffer;
		
		fread_s(charBuffer.data(), bufferSize, 1, bufferSize, ptr);
		charBuffer.push_back('\0');
		if (strcmp(reinterpret_cast<const char*>(charBuffer.data()), "RIFF") == 0) {
			printf("RIFF HEADER CORRECT!\n");
			fseek(ptr, 8, SEEK_SET);
			charBuffer.clear();
			charBuffer.resize(bufferSize);
			fread_s(charBuffer.data(), bufferSize, 1, bufferSize, ptr);
			charBuffer.push_back('\0');

			if (strcmp(reinterpret_cast<const char*>(charBuffer.data()), "WAVE") == 0) {
				printf("WAVE HEADER CORRECT!\n");
			
				charBuffer.clear();
				charBuffer.resize(bufferSize);
				fread_s(charBuffer.data(), bufferSize, 1, bufferSize, ptr);
				charBuffer.push_back('\0');

				if (strcmp(reinterpret_cast<const char*>(charBuffer.data()), "fmt ") == 0) {
					printf("fmt HEADER CORRECT!\n");
					
					//VALID WAV!
					fseek(ptr, currentChar, SEEK_SET);
					fread_s(&fmtChunkSize, sizeof(long), 1, sizeof(long), ptr);

					if (fmtChunkSize > 16)
						fseek(ptr, 36, SEEK_SET);

					fread_s(&extraParamSize, sizeof(short), 1, sizeof(short), ptr);
					fseek(ptr, currentChar + extraParamSize, SEEK_SET);
					charBuffer.clear();
					charBuffer.resize(4);
					while ((!feof(ptr)) && (!foundData)) {
						charBuffer.clear();
						charBuffer.resize(4);
						fread_s(charBuffer.data(), bufferSize, 1, bufferSize, ptr);
						charBuffer.push_back('\0');
						if (strcmp(reinterpret_cast<const char*>(charBuffer.data()), "data") == 0) {
							fread_s(&dataSize, sizeof(long), 1, sizeof(long), ptr);
							std::cout << "Found data header! Data size: " << dataSize;
							foundData = true;
						}
						else {
							currentChar++;
							fseek(ptr, currentChar, SEEK_SET);
						}
					}

					dataBuffer.resize(dataSize);
					fread_s(dataBuffer.data(), dataSize, 1, dataSize, ptr);
					printf("%s\n", dataBuffer.data());
					fseek(ptr, 24, SEEK_SET);
					fread_s(&sampleRate, sizeof(long), 1, sizeof(long), ptr);
					std::cout << "SampleRate: " << sampleRate << std::endl;
					fseek(ptr, 34, SEEK_SET);
					fread_s(&bitsPerSample, sizeof(short), 1, sizeof(short), ptr);
					std::cout << "bitsPerSample: " << bitsPerSample << std::endl;
					fseek(ptr, 22, SEEK_SET);
					fread_s(&channels, sizeof(short), 1, sizeof(short), ptr);
					std::cout << "channels: " << channels << std::endl;


					resultAudioBuffer = new AudioBuffer();
					alGenBuffers(1, &bufferId);
					resultAudioBuffer->bufferId = bufferId;

					if (bitsPerSample == 8) {
						if (channels == 1)
							alBufferData(bufferId, AL_FORMAT_MONO8, dataBuffer.data(), dataSize, sampleRate);
						else
							alBufferData(bufferId, AL_FORMAT_STEREO8, dataBuffer.data(), dataSize, sampleRate);

					}
					else if (bitsPerSample == 16) {
						if (channels == 1)
							alBufferData(bufferId, AL_FORMAT_MONO16, dataBuffer.data(), dataSize, sampleRate);
						else
							alBufferData(bufferId, AL_FORMAT_STEREO16, dataBuffer.data(), dataSize, sampleRate);
					}
				}
				else {
					printf("Invalid WAV File!");
				}
			}
			else {
				printf("Invalid WAV File!");
			}
			
		}
		else {
			printf("Invalid WAV File!");
		}		
			
		fclose(ptr);	


	}

	return resultAudioBuffer;
}

AudioBuffer::~AudioBuffer() {
	alDeleteBuffers(1, &bufferId);

}