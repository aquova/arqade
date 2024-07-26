#include "Arqade.hpp"

#include <SDL2/SDL_timer.h>
#include <QString>

#include "AudioPlayer.hpp"
#include "Core.hpp"
#include "CoreData.hpp"
#include "Database.hpp"
#include "VideoPlayer.hpp"

static constexpr auto SCALE = 3;
static constexpr auto FPS = 60.0;
static constexpr auto FRAME_DELAY = 1000.0 / FPS;

void Arqade::RunEmu(QString aRomPath, QString aCorePath, ControllerMapType aBindings) {
    CoreData::getInstance().Reset();
    Core core(aRomPath.toStdString(), aCorePath.toStdString());
    AudioPlayer audio_player;
    const auto av_info = CoreData::getInstance().GetAvInfo();
    VideoPlayer video_player(av_info.mGeometry.mBaseWidth, av_info.mGeometry.mBaseHeight, SCALE, aBindings);

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
