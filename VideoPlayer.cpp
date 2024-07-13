#include "VideoPlayer.hpp"

#include <cstring>
#include <iostream>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>

#include "CoreData.hpp"
#include "Utils.hpp"

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

VideoPlayer::VideoPlayer(const int aWidth, const int aHeight, const int aScale) : mWidth(aWidth), mHeight(aHeight), mScale(aScale) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Unable to initialize video\n";
    }

    mWindow = SDL_CreateWindow("Arqade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth * mScale, mHeight * mScale, SDL_WINDOW_SHOWN);
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, mWidth, mHeight);
}

VideoPlayer::~VideoPlayer() {
    SDL_DestroyTexture(mTexture);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

Hotkey VideoPlayer::HandleButton(const SDL_Keycode aKey, const bool aPressed) {
    auto button = JoypadButton::COUNT;
    // TODO: Don't make this hardcoded
    switch (aKey) {
        case SDLK_UP:
            button = JoypadButton::UP;
            break;

        case SDLK_DOWN:
            button = JoypadButton::DOWN;
            break;

        case SDLK_LEFT:
            button = JoypadButton::LEFT;
            break;

        case SDLK_RIGHT:
            button = JoypadButton::RIGHT;
            break;

        case SDLK_z:
            button = JoypadButton::SOUTH;
            break;

        case SDLK_x:
            button = JoypadButton::EAST;
            break;

        case SDLK_s:
            button = JoypadButton::NORTH;
            break;

        case SDLK_a:
            button = JoypadButton::WEST;
            break;

        case SDLK_q:
            button = JoypadButton::L;
            break;

        case SDLK_w:
            button = JoypadButton::R;
            break;

        case SDLK_RETURN:
            button = JoypadButton::START;
            break;

        case SDLK_BACKSPACE:
            button = JoypadButton::SELECT;
            break;

        case SDLK_F5:
            if (aPressed) {
                return Hotkey::SaveState;
            }
            break;

        case SDLK_F9:
            if (aPressed) {
                return Hotkey::LoadState;
            }
            break;

        default:
            break;
    }

    if (button != JoypadButton::COUNT) {
        CoreData::getInstance().SetButtonPress(button, aPressed);
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
    const auto converted = ConvertPixelFormat(aBuffer, aFormat);

    int pitch = 0;
    void* pixels = nullptr;

    SDL_LockTexture(mTexture, nullptr, &pixels, &pitch);
    std::memcpy(pixels, converted.data(), pitch * mHeight);
    SDL_UnlockTexture(mTexture);

    SDL_Rect source_rect{0, 0, mWidth, mHeight};
    SDL_RenderCopy(mRenderer, mTexture, &source_rect, nullptr);
    SDL_RenderPresent(mRenderer);
}

void VideoPlayer::SetTitle(const std::string aTitle) {
    SDL_SetWindowTitle(mWindow, aTitle.c_str());
}
