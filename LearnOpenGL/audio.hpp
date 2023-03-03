#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string_view>
#include <memory>
#include <array>

namespace wava
{
    class WavAudio
    {
    private:
        uint32_t buffer;
        bool loop = true;
        bool loaded = false;

        // RIFF chunk (main chunk)
        uint32_t chunkSize;
        char format[5] = { '\0' };

        // sub-chunk 1 (fmt chunk)
        uint32_t subChunk1Size;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;

        // sub-chunk 2 (data)
        uint32_t subChunk2Size;
        unsigned char* data;

        int getFileCursorMark(std::ifstream& fs, std::string mark);

    public:
        WavAudio();
        WavAudio(const char* path);
        ~WavAudio();

        void load(const char* path);
        uint32_t getBuffer();
    };
}

namespace flat
{
    // abandoned
	class AudioSource
	{
	private:
        ALuint source = 0;
        void releaseAudioSource();
	public:
		AudioSource();
		~AudioSource();
        void initialize();
		void play(uint32_t buffer);
        void play();
        void stop();
        int getBuffer();
        bool getLoopable();
        std::array<float, 3> getPostion();
        std::array<float, 3> getVelocity();
        void setLoopable(bool b);
        void setPosition(float x, float y, float z);
        void setVelocity(float x, float y, float z);
        void setBuffer(uint32_t buffer);
	};
}