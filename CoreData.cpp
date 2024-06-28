#include "CoreData.hpp"

void CoreData::EraseAudioData(const size_t aLength) {
    const auto len = std::min(aLength, mAudioData.size());
    mAudioData.erase(mAudioData.begin(), mAudioData.begin() + len);
}

std::vector<int16_t> CoreData::GetAudioData() {
    return mAudioData;
}

int CoreData::GetBytesPerPixel() {
    switch (mPixelFormat) {
        case PixelFormat::XRGB8888:
            return 4;

        case PixelFormat::RGB565:
        case PixelFormat::RGB1555:
            return 2;

        case PixelFormat::UNKNOWN:
        default:
            return 0;
    }
}

PixelFormat CoreData::GetPixelFormat() {
    return mPixelFormat;
}

uint CoreData::GetScreenHeight() {
    return mScreenHeight;
}

size_t CoreData::GetScreenPitch() {
    return mScreenPitch;
}

uint CoreData::GetScreenWidth() {
    return mScreenWidth;
}

bool CoreData::IsButtonPressed(const JoypadButton aButton) {
    return mButtons[aButton];
}

std::vector<uint8_t> CoreData::GetVideoData() {
    return mVideoData;
}

void CoreData::SetAudioData(std::vector<int16_t> aData) {
    mAudioData.insert(mAudioData.end(), aData.begin(), aData.end());
}

void CoreData::SetButtonPress(const JoypadButton aButton, const bool aPressed) {
    mButtons[aButton] = aPressed;
}

void CoreData::SetPixelFormat(const PixelFormat aFormat) {
    mPixelFormat = aFormat;
}

void CoreData::SetScreenParams(const uint aWidth, const uint aHeight, const size_t aPitch) {
    mScreenWidth = aWidth;
    mScreenHeight = aHeight;
    mScreenPitch = aPitch;
}

void CoreData::SetVideoData(std::vector<uint8_t> aData) {
    mVideoData = aData;
}
