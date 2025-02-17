#ifndef COREDATA_HPP
#define COREDATA_HPP

#include <array>
#include <mutex>
#include <sys/types.h>
#include <vector>

#include "libretro.hpp"

struct ScreenParams {
    int mWidth;
    int mHeight;
    size_t mPitch;
};

class CoreData {
    public:
        static CoreData& getInstance() {
            static CoreData instance;
            return instance;
        }

        void EraseAudioData(const size_t aLength);
        std::vector<int16_t> GetAudioData();
        AvInfo GetAvInfo();
        PixelFormat GetPixelFormat();
        ScreenParams GetScreenParams();
        std::vector<uint8_t> GetVideoData();
        bool IsButtonPressed(const JoypadButton aButton);
        void Reset();
        void SetAudioData(std::vector<int16_t> aData);
        void SetAvInfo(AvInfo aAvInfo);
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

        AvInfo mAvInfo;
        std::array<bool, static_cast<int>(JoypadButton::COUNT)> mButtons;
        PixelFormat mPixelFormat{PixelFormat::UNKNOWN};
        ScreenParams mScreenParams;

        std::vector<int16_t> mAudioData;
        std::vector<uint8_t> mVideoData;
};

#endif
