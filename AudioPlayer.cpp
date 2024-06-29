#include "AudioPlayer.hpp"

#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>

#include "CoreData.hpp"

static void AudioCallback(void* aUserdata, uint8_t* aStream, int aLength) {
    CoreData& core_data = CoreData::getInstance();
    const auto buffer = core_data.GetAudioData();
    const auto data_len = std::min(static_cast<size_t>(aLength), buffer.size());

    if (data_len > 0) {
        memcpy(aStream, buffer.data(), data_len);
        // If we don't have enough data requested, pad
        if (data_len < aLength) {
            memset(aStream + data_len, 0, aLength - data_len);
        }
        core_data.EraseAudioData(data_len);
    } else {
        memset(aStream, 0, aLength);
    }
}

AudioPlayer::AudioPlayer() {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "Failure to initialize audio\n";
    }

    SDL_AudioSpec desired_spec, obtained_spec;
    const auto av_info = CoreData::getInstance().GetAvInfo();
    desired_spec.freq = av_info.mTiming.mSampleRate;
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
