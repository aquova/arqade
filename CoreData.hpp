#ifndef COREDATA_HPP
#define COREDATA_HPP

#include <array>
#include <mutex>
#include <sys/types.h>
#include <vector>

#include "libretro.hpp"

class CoreData {
    public:
        static CoreData& getInstance() {
            static CoreData instance;
            return instance;
        }

        void EraseAudioData(const size_t aLength);
        std::vector<int16_t> GetAudioData();
        int GetBytesPerPixel();
        PixelFormat GetPixelFormat();
        size_t GetScreenPitch();
        uint GetScreenHeight();
        uint GetScreenWidth();
        std::vector<uint8_t> GetVideoData();
        bool IsButtonPressed(const JoypadButton aButton);
        void SetAudioData(std::vector<int16_t> aData);
        void SetButtonPress(const JoypadButton aButton, const bool aPressed);
        void SetScreenParams(const uint aWidth, const uint aHeight, const size_t aPitch);
        void SetPixelFormat(const PixelFormat aFormat);
        void SetVideoData(std::vector<uint8_t> aData);

    private:
        CoreData() = default;
        ~CoreData() = default;
        CoreData(const CoreData&) = delete;
        CoreData& operator=(const CoreData&) = delete;

        std::mutex mMutex;
        std::array<bool, JoypadButton::COUNT> mButtons;
        PixelFormat mPixelFormat{PixelFormat::UNKNOWN};
        uint mScreenHeight{0};
        size_t mScreenPitch{0};
        uint mScreenWidth{0};
        std::vector<int16_t> mAudioData;
        std::vector<uint8_t> mVideoData;
};

#endif
