#include "AudioPlayer.hpp"

#include <SDL2/SDL_timer.h>
#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>

#include "CoreData.hpp"

// TODO: Need to get sample rate from core
static constexpr auto SAMPLE_RATE = 44100;
static constexpr auto AMPLITUDE = 28000;
static constexpr auto FREQ = 440;
static constexpr auto SAMPLES = 4096;

static void AudioCallback(void* aUserdata, uint8_t* aStream, int aLength) {
    CoreData& core_data = CoreData::getInstance();
    const auto buffer = core_data.GetAudioData();
    if (buffer.size() == 0 || !aStream) {
        return;
    }
    const auto data_len = std::min(static_cast<size_t>(aLength), buffer.size());
    memcpy(aStream, buffer.data(), data_len);
    core_data.EraseAudioData(data_len);
}

AudioPlayer::AudioPlayer() {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "Failure to initialize audio\n";
    }

    SDL_AudioSpec desired_spec, obtained_spec;
    desired_spec.freq = SAMPLE_RATE;
    desired_spec.format = AUDIO_S16;
    desired_spec.channels = 2;
    desired_spec.callback = AudioCallback;
    desired_spec.userdata = nullptr;

    mAudioDevice = SDL_OpenAudioDevice(nullptr, 0, &desired_spec, &obtained_spec, 0);
    if (!mAudioDevice) {
        std::cerr << "Failed to open audio: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_PauseAudioDevice(mAudioDevice, 0);
}

AudioPlayer::~AudioPlayer() {
    SDL_CloseAudioDevice(mAudioDevice);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void AudioPlayer::Kill() {
    mRunning = false;
}

void AudioPlayer::Run() {
    mRunning = true;
    while (mRunning) {
        SDL_Delay(100);
    }
}
