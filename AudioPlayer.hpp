#ifndef AUDIOPLAYER_HPP
#define AUDIOPLAYER_HPP

#include <cstdint>
#include <vector>

#include <SDL2/SDL_audio.h>

class AudioPlayer {
    public:
        AudioPlayer();
        ~AudioPlayer();
        void Kill();
        void Run();

    private:
        std::vector<int16_t> mAudioBuffer;
        SDL_AudioDeviceID mAudioDevice;

        bool mRunning{false};
};

#endif
