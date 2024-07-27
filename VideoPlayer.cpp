#include "VideoPlayer.hpp"

#include <cstring>
#include <exception>
#include <iostream>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>

#include "CoreData.hpp"
#include "Database.hpp"
#include "Utils.hpp"

static int GetBytesPerPixel(const PixelFormat aFormat) {
    switch (aFormat) {
        case PixelFormat::RGB565:
        case PixelFormat::RGB1555:
            return 2;

        case PixelFormat::XRGB8888:
            return 4;

        default:
            return 0;
    }
}

static std::vector<uint8_t> ConvertRGB1555ToRGB888(const std::vector<uint8_t> &aVector) {
    const auto bytes_per_pixel = GetBytesPerPixel(PixelFormat::RGB1555);
    std::vector<uint8_t> output;
    for (size_t i = 0; i < aVector.size(); i += bytes_per_pixel) {
        const auto first = aVector.at(i);
        const auto second = aVector.at(i + 1);

        const auto red = (first & 0b01111100) >> 2;
        const auto green = ((first & 0b11) << 3) | ((second & 0b11100000) >> 5);
        const auto blue = second & 0b00011111;

        const auto scaled_red = static_cast<uint8_t>(red * 255.0 / 31.0);
        const auto scaled_green = static_cast<uint8_t>(green * 255.0 / 31.0);
        const auto scaled_blue = static_cast<uint8_t>(blue * 255.0 / 31.0);

        output.push_back(scaled_red);
        output.push_back(scaled_green);
        output.push_back(scaled_blue);
    }
    return output;
}

static std::vector<uint8_t> ConvertRGB565ToRGB888(const std::vector<uint8_t> &aVector) {
    const auto bytes_per_pixel = GetBytesPerPixel(PixelFormat::RGB565);
    std::vector<uint8_t> output;
    for (size_t i = 0; i < aVector.size(); i += bytes_per_pixel) {
        // I assume this is an endian thing
        const auto second = aVector.at(i);
        const auto first = aVector.at(i + 1);

        const auto red = (first & 0b11111000) >> 3;
        const auto green = ((first & 0b111) << 3) | ((second & 0b11100000) >> 5);
        const auto blue = second & 0b00011111;

        const auto scaled_red = static_cast<uint8_t>(red * 255.0 / 31.0);
        const auto scaled_green = static_cast<uint8_t>(green * 255.0 / 63.0);
        const auto scaled_blue = static_cast<uint8_t>(blue * 255.0 / 31.0);

        output.push_back(scaled_red);
        output.push_back(scaled_green);
        output.push_back(scaled_blue);
    }
    return output;
}

static std::vector<uint8_t> ConvertRGB8888ToRGB888(const std::vector<uint8_t> &aVector) {
    const auto bytes_per_pixel = GetBytesPerPixel(PixelFormat::XRGB8888);
    std::vector<uint8_t> output;
    for (size_t i = 0; i < aVector.size(); i += bytes_per_pixel) {
        // Despite what XRGB8888 might imply, it's actually XBGR8888
        output.push_back(aVector.at(i + 2));
        output.push_back(aVector.at(i + 1));
        output.push_back(aVector.at(i + 0));
    }
    return output;
}

static std::vector<uint8_t> ConvertPixelFormat(const std::vector<uint8_t> &aData, const PixelFormat aFormat) {
    std::vector<uint8_t> output;
    switch (aFormat) {
        case PixelFormat::RGB1555:
            return ConvertRGB1555ToRGB888(aData);

        case PixelFormat::RGB565:
            return ConvertRGB565ToRGB888(aData);

        case PixelFormat::XRGB8888:
            return ConvertRGB8888ToRGB888(aData);

        case PixelFormat::UNKNOWN:
        default:
            // TODO: Throw error
            std::cout << "Unknown pixel format\n";
            std::vector<uint8_t> tmp;
            return tmp;
    }
}

VideoPlayer::VideoPlayer(const int aWidth, const int aHeight, const int aScale, const ControllerMapType aBindings) : mWidth(aWidth), mHeight(aHeight), mScale(aScale), mBindings(aBindings) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Unable to initialize video\n";
    }

    mWindow = SDL_CreateWindow("Arqade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth * mScale, mHeight * mScale, SDL_WINDOW_SHOWN);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
}

VideoPlayer::~VideoPlayer() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
    }
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

Hotkey VideoPlayer::HandleButton(const SDL_Keycode aKey, const bool aPressed) {
    const auto qt = SDL2Qt(aKey);
    try {
        const auto button = mBindings.at(qt);
        CoreData::getInstance().SetButtonPress(button, aPressed);
    } catch (const std::exception& e) {
        // Unset key was pressed, do nothing
    }
    return Hotkey::None;
}

Hotkey VideoPlayer::HandleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return Hotkey::Quit;

            case SDL_KEYDOWN:
                HandleButton(event.key.keysym.sym, true);
                break;

            case SDL_KEYUP:
                HandleButton(event.key.keysym.sym, false);
                break;

            default:
                break;
        }
    }
    return Hotkey::None;
}

void VideoPlayer::Render(const std::vector<uint8_t> &aBuffer, const PixelFormat aFormat) {
    const auto screen_params = CoreData::getInstance().GetScreenParams();
    const auto bytes_per_pixel = GetBytesPerPixel(aFormat);

    if (mTexture == nullptr) {
        mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, screen_params.mPitch / bytes_per_pixel, screen_params.mHeight);
    }
    const auto converted = ConvertPixelFormat(aBuffer, aFormat);

    int pitch = 0;
    void* pixels = nullptr;

    SDL_LockTexture(mTexture, nullptr, &pixels, &pitch);
    std::memcpy(pixels, converted.data(), converted.size());
    SDL_UnlockTexture(mTexture);

    SDL_Rect source_rect{0, 0, mWidth, mHeight};
    SDL_RenderCopy(mRenderer, mTexture, &source_rect, nullptr);
    SDL_RenderPresent(mRenderer);
}

void VideoPlayer::SetTitle(const std::string aTitle) {
    SDL_SetWindowTitle(mWindow, aTitle.c_str());
}
