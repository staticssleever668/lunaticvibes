#pragma once
#include <array>
#include <string>
#include "fmod.hpp"

typedef std::size_t size_t;

enum class DSPType
{
    OFF,
    REVERB,
    DELAY,
    LOWPASS,
    HIGHPASS,
    FLANGER,
    CHORUS,
    DISTORTION,
};

enum class EQFreq
{
    _62_5,
    _160,
    _400,
    _1000,
    _2500,
    _6250,
    _16k
};

enum class SampleChannel
{
    MASTER,
    KEY,
    BGM,
};

class SoundDriver
{
    friend class SoundMgr;

public:
    SoundDriver() = default;
    virtual ~SoundDriver() = default;

public:
    virtual std::vector<std::pair<int, std::string>> getDeviceList(bool asio = false) = 0;

public:
    static constexpr size_t KEYSAMPLES = 36 * 36 + 1;
    static constexpr size_t ETCSAMPLES = 64; 

protected:
    std::array<FMOD::Sound*, KEYSAMPLES> keySamples{};  // Sound samples of key sound
    std::array<FMOD::Sound*, ETCSAMPLES> etcSamples{};  // Sound samples of BGM, effect, etc

public:
    virtual int loadKeySample(const Path& path, size_t index) = 0;
    virtual void playKeySample(size_t count, size_t index[]) = 0;
    virtual void stopKeySamples() = 0;
    virtual void freeKeySamples() = 0;
    virtual int loadSample(const Path& path, size_t index, bool isStream = false, bool loop = false) = 0;
    virtual void playSample(size_t index) = 0;
    virtual void stopSamples() = 0;
    virtual void freeSamples() = 0;
    virtual void update() = 0;

public:
    virtual void setVolume(SampleChannel ch, float v) = 0;
    virtual void setDSP(DSPType type, int index, SampleChannel ch, float p1, float p2) = 0;
    virtual void setFreqFactor(double f) = 0;
    virtual void setSpeed(double speed) = 0;
    virtual void setPitch(double pitch) = 0;
    virtual void setEQ(EQFreq freq, int gain) = 0;
};
