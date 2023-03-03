#include "audio.hpp"

#include <iostream>
#include <fstream>
#include <array>

flat::AudioSource::AudioSource()
{

}

flat::AudioSource::~AudioSource()
{
    releaseAudioSource();
}

void flat::AudioSource::releaseAudioSource()
{
    alDeleteSources(1, &source);
}

void flat::AudioSource::initialize()
{
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1.0f);
    alSourcef(source, AL_GAIN, 1.0f);
}

void flat::AudioSource::play(uint32_t buffer)
{
    setBuffer(buffer);
    play();
}

void flat::AudioSource::play()
{
    alSourcePlay(source);
}

void flat::AudioSource::stop()
{
    alSourceStop(source);
}

void flat::AudioSource::setBuffer(uint32_t buffer)
{
    alSourcei(source, AL_BUFFER, buffer);
}

void flat::AudioSource::setVelocity(float x, float y, float z)
{
    alSource3f(source, AL_VELOCITY, x, y, z);
}

void flat::AudioSource::setPosition(float x, float y, float z)
{
    alSource3f(source, AL_POSITION, x, y, z);
}

void flat::AudioSource::setLoopable(bool b)
{
    alSourcei(source, AL_LOOPING, b);
}

int flat::AudioSource::getBuffer()
{
    int buffer;
    alGetSourcei(source, AL_BUFFER, &buffer);
    return buffer;
}

bool flat::AudioSource::getLoopable()
{
    int val;
    alGetSourcei(source, AL_LOOPING, &val);
    return static_cast<bool>(val);
}

std::array<float, 3> flat::AudioSource::getPostion()
{
    std::array<float, 3> buffer;
    alGetSourcefv(source, AL_POSITION, buffer.data());
    return buffer;
}

std::array<float, 3> flat::AudioSource::getVelocity()
{
    std::array<float, 3> buffer;
    alGetSourcefv(source, AL_VELOCITY, buffer.data());
    return buffer;
}

int wava::WavAudio::getFileCursorMark(std::ifstream& fs, std::string mark)
{
    int len = mark.length();
    // dynamic C-style array may not work on msvc
    // if that happens, fix it by yourself
    // it's sample, good luck
    char* buf = new char[len + 1];
    buf[len] = '\0';
    int i = 0;
    while (!fs.eof())
    {
        fs.seekg(i++, std::ios::beg);
        fs.read(buf, sizeof(char) * len);
        if (mark.compare(buf) == 0)
        {
            delete[] buf;
            return i;
        }
    }
    std::cerr << "[libwavaudio] ERROR: failed to locate mark (" << mark << ") in moveFileCursorToMark()\n";
    abort();

}

wava::WavAudio::WavAudio()
{

}

wava::WavAudio::WavAudio(const char* path)
{
    load(path);
}

wava::WavAudio::~WavAudio()
{
    alDeleteBuffers(1, &buffer);
}

void wava::WavAudio::load(const char* path)
{
    int i;
    std::ifstream fs(path, std::ios::in | std::ios::binary);
    if (!fs)
    {
        std::cerr << "[libwavaudio] ERROR: can't open file (" << path << ")\n";
        abort();
    }

    i = getFileCursorMark(fs, "RIFF") - 1;
    fs.seekg(i + 4, std::ios::beg);
    fs.read((char*)&chunkSize, 4);
    fs.seekg(i + 8, std::ios::beg);
    fs.read((char*)&format, 4);

    if (std::string(format).compare("WAVE") != 0)
    {
        std::cerr << "[libwavaudio] ERROR: trying to load a none-wav format file (" << path << ")\n";
        abort();
    }

    i = getFileCursorMark(fs, "fmt") - 1;
    fs.seekg(i + 4, std::ios::beg);
    fs.read((char*)&subChunk1Size, 4);
    fs.seekg(i + 8, std::ios::beg);
    fs.read((char*)&audioFormat, 2);
    fs.seekg(i + 10, std::ios::beg);
    fs.read((char*)&numChannels, 2);
    fs.seekg(i + 12, std::ios::beg);
    fs.read((char*)&sampleRate, 4);
    fs.seekg(i + 16, std::ios::beg);
    fs.read((char*)&byteRate, 4);
    fs.seekg(i + 20, std::ios::beg);
    fs.read((char*)&blockAlign, 2);
    fs.seekg(i + 22, std::ios::beg);
    fs.read((char*)&bitsPerSample, 2);
    fs.seekg(i + 24, std::ios::beg);

    i = getFileCursorMark(fs, "data") - 1;
    fs.seekg(i + 4, std::ios::beg);
    fs.read((char*)&subChunk2Size, 4);
    fs.seekg(i + 8, std::ios::beg);
    data = new unsigned char[subChunk2Size];
    fs.read((char*)data, subChunk2Size);

    // load data to OpenAL buffer
    alGenBuffers(1, &buffer);
    if (bitsPerSample == 16)
    {
        if (numChannels == 1)
            alBufferData(buffer, AL_FORMAT_MONO16, data, subChunk2Size, sampleRate);
        else if (numChannels == 2)
            alBufferData(buffer, AL_FORMAT_STEREO16, data, subChunk2Size, sampleRate);
        else
            abort();
    }
    else if (bitsPerSample == 8)
    {
        if (numChannels == 1)
            alBufferData(buffer, AL_FORMAT_MONO8, data, subChunk2Size, sampleRate);
        else if (numChannels == 2)
            alBufferData(buffer, AL_FORMAT_STEREO8, data, subChunk2Size, sampleRate);
        else
            abort();
    }
    else
        abort();

    // release data
    delete[] data;
    fs.close();

    loaded = true;
}

uint32_t wava::WavAudio::getBuffer()
{
    if (loaded)
        return buffer;
    else
    {
        std::cerr << "[libwavaudio] ERROR: called getBuffer() from an unloaded WavAudio\n";
        abort();
    }
}