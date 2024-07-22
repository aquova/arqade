#ifndef VIDEOPLAYER_HPP
#define VIDEOPLAYER_HPP

#include <string>
#include <vector>

#include <Qt>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "Database.hpp"
#include "libretro.hpp"

enum class Hotkey {
    None,
    Quit,
    Reset,
    SaveState,
    LoadState,
};

class VideoPlayer {
    public:
        VideoPlayer(const int aWidth, const int aHeight, const int aScale, const ControllerMapType aBindings);
        ~VideoPlayer();
        Hotkey HandleInput();
        void Render(const std::vector<uint8_t> &aBuffer, const PixelFormat aFormat);
        void SetTitle(const std::string aTitle);

    private:
        Hotkey HandleButton(const SDL_Keycode aKey, const bool aPressed);

        int mWidth;
        int mHeight;
        int mScale;
        int mGuiTab;
        ControllerMapType mBindings;
        SDL_Renderer* mRenderer{nullptr};
        SDL_Texture* mTexture{nullptr};
        SDL_Window* mWindow{nullptr};
};

#endif
