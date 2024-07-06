#include <SDL2/SDL_timer.h>

#include "AudioPlayer.hpp"
#include "Core.hpp"
#include "CoreData.hpp"
#include "VideoPlayer.hpp"

// TODO: These shouldn't be hardcoded
static constexpr auto WIDTH = 256; // Needs to be equal to pitch, not screen width for now
static constexpr auto HEIGHT = 224;
static constexpr auto SCALE = 3;
// const auto WIDTH = 347;
// const auto HEIGHT = 244;
// const auto SCALE = 3;

static constexpr auto FPS = 60.0;
static constexpr auto FRAME_DELAY = 1000.0 / FPS;

void RunEmu(std::string aRomPath, std::string aCorePath) {
    Core core(aRomPath, aCorePath);
    AudioPlayer audio_player;
    VideoPlayer video_player(WIDTH, HEIGHT, SCALE);

    while (true) {
        const auto frame_start = SDL_GetTicks();
        core.RunGame();

        const auto input = video_player.HandleInput();
        if (input == Hotkey::Quit) {
            break;
        }

        CoreData& core_data = CoreData::getInstance();
        const auto pixels = core_data.GetVideoData();
        const auto format = core_data.GetPixelFormat();
        video_player.Render(pixels, format);

        const auto frame_time = SDL_GetTicks() - frame_start;
        const auto delay_time = FRAME_DELAY - frame_time;
        if (delay_time > 0) {
            SDL_Delay(delay_time);
        }
    }

    core.WriteSave();
}
