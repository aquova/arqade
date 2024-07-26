#include "Core.hpp"

#include "CoreData.hpp"
#include "libretro.hpp"

#include <cstdint>
#include <dlfcn.h>
#include <fstream>
#include <iostream>

static constexpr auto LIBRETRO_VERSION = 1;

typedef void (*RetroInitType)();
typedef uint (*RetroApiVersionType)();

typedef bool (*SetEnvCallbackType)(uint aCommand, void* aData);
typedef void (*SetVideoRefreshCallbackType)(void* aData, uint aWidth, uint aHeight, size_t aPitch);
typedef void (*SetInputPollCallbackType)();
typedef int16_t (*SetInputStateCallbackType)(uint aPort, uint aDevice, uint aIndex, uint aId);
typedef void (*SetAudioSampleCallbackType)(int16_t aLeft, int16_t aRight);
typedef size_t (*SetAudioSampleBatchCallbackType)(int16_t* aData, size_t aFrames);

typedef void (*RetroSetEnvironmentType)(SetEnvCallbackType aCallback);
typedef void (*RetroSetVideoRefreshType)(SetVideoRefreshCallbackType aCallback);
typedef void (*RetroSetInputPollType)(SetInputPollCallbackType aCallback);
typedef void (*RetroSetInputStateType)(SetInputStateCallbackType aCallback);
typedef void (*RetroSetAudioSampleType)(SetAudioSampleCallbackType aCallback);
typedef void (*RetroSetAudioSampleBatchType)(SetAudioSampleBatchCallbackType aCallback);

bool SetEnvCallback(uint aCommand, void* aData) {
    switch (aCommand) {
        case RETRO_ENV_GET_CAN_DUPE:
            *static_cast<bool*>(aData) = true;
            break;

        case RETRO_ENV_SET_PIXEL_FORMAT: {
            const auto format = *static_cast<PixelFormat*>(aData);
            CoreData::getInstance().SetPixelFormat(format);
            return true;
        }

        default:
            // std::cout << aCommand << '\n';
            break;
    }
    return false;
}

void SetVideoRefreshCallback(void* aData, uint aWidth, uint aHeight, size_t aPitch) {
    if (!aData) {
        std::cout << "No Video Data\n";
        return;
    }

    CoreData& core_data = CoreData::getInstance();
    core_data.SetScreenParams(aWidth, aHeight, aPitch);
    std::vector<uint8_t> data_vec;
    const auto buffer_len = aPitch * aHeight;
    const auto buffer_data = static_cast<uint8_t*>(aData);

    data_vec.insert(data_vec.end(), &buffer_data[0], &buffer_data[buffer_len]);
    core_data.SetVideoData(data_vec);
}

void SetInputPollCallback() {
    // std::cout << "SetInputPollCallback\n";
}

int16_t SetInputStateCallback(uint aPort, uint aDevice, uint aIndex, uint aId) {
    // TODO: Need to handle other controllers in other ports
    // Assume only referring to controller 1 for now
    const auto button = static_cast<JoypadButton>(aId);
    const auto isPressed = CoreData::getInstance().IsButtonPressed(button);
    return isPressed ? 1 : 0;
}

void SetAudioSampleCallback(int16_t aLeft, int16_t aRight) {
    // Nothing seems to use this..
    std::cout << "SetAudioSampleCallback\n";
}

size_t SetAudioSampleBatchCallback(int16_t* aData, size_t aFrames) {
    std::vector<int16_t> data_vec;
    if (!aData) {
        std::cout << "Invalid audio buffer\n";
        CoreData::getInstance().SetAudioData(data_vec);
        return 0;
    }
    const auto data_len = aFrames * 2; // Two channels
    data_vec.insert(data_vec.end(), &aData[0], &aData[data_len]);
    CoreData::getInstance().SetAudioData(data_vec);
    return data_len;
}

Core::Core(std::string aGamePath, std::string aLibPath) : mGamePath(aGamePath), mLibPath(aLibPath) {
    mSavePath = aGamePath.substr(0, aGamePath.find_last_of('.')) + ".sav";

    AvInfo av_info;
    InitCore();
    LoadGame();
    mRetroGetSystemAvInfoFn(&av_info);
    CoreData::getInstance().SetAvInfo(av_info);
    LoadSave();
}

Core::~Core() {
    if (mCoreLib) {
        dlclose(mCoreLib);
    }
}

void Core::InitCore() {
    mCoreLib = dlopen(mLibPath.c_str(), RTLD_LAZY | RTLD_NOW);
    if (!mCoreLib) {
        std::cerr << "Unable to open shared object\n";
        return;
    }
    dlerror();

    auto retro_init = reinterpret_cast<RetroInitType>(dlsym(mCoreLib, "retro_init"));
    auto retro_api_version = reinterpret_cast<RetroApiVersionType>(dlsym(mCoreLib, "retro_api_version"));
    auto retro_set_environment = reinterpret_cast<RetroSetEnvironmentType>(dlsym(mCoreLib, "retro_set_environment"));
    auto retro_set_video_refresh = reinterpret_cast<RetroSetVideoRefreshType>(dlsym(mCoreLib, "retro_set_video_refresh"));
    auto retro_set_input_poll = reinterpret_cast<RetroSetInputPollType>(dlsym(mCoreLib, "retro_set_input_poll"));
    auto retro_set_input_state = reinterpret_cast<RetroSetInputStateType>(dlsym(mCoreLib, "retro_set_input_state"));
    auto retro_set_audio_sample = reinterpret_cast<RetroSetAudioSampleType>(dlsym(mCoreLib, "retro_set_audio_sample"));
    auto retro_set_audio_sample_batch = reinterpret_cast<RetroSetAudioSampleBatchType>(dlsym(mCoreLib, "retro_set_audio_sample_batch"));

    mRetroLoadFn = reinterpret_cast<RetroLoadGame>(dlsym(mCoreLib, "retro_load_game"));
    mRetroRunFn = reinterpret_cast<RetroRunType>(dlsym(mCoreLib, "retro_run"));
    mRetroGetMemoryDataFn = reinterpret_cast<RetroGetMemoryDataType>(dlsym(mCoreLib, "retro_get_memory_data"));
    mRetroGetMemorySizeFn = reinterpret_cast<RetroGetMemorySizeType>(dlsym(mCoreLib, "retro_get_memory_size"));
    mRetroGetSystemAvInfoFn = reinterpret_cast<RetroGetSystemAvInfoType>(dlsym(mCoreLib, "retro_get_system_av_info"));

    const auto dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Unable to load function " << dlsym_error << '\n';
        return;
    }
    const auto api_version = retro_api_version();
    if (api_version != LIBRETRO_VERSION) {
        std::cout << "Unexpect API version\n";
    }

    retro_set_environment(SetEnvCallback);
    retro_init();
    retro_set_video_refresh(SetVideoRefreshCallback);
    retro_set_input_poll(SetInputPollCallback);
    retro_set_input_state(SetInputStateCallback);
    retro_set_audio_sample(SetAudioSampleCallback);
    retro_set_audio_sample_batch(SetAudioSampleBatchCallback);
}

void Core::LoadGame() {
    std::ifstream file(mGamePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        return;
    }
    file.seekg(0, std::ios::end);
    const auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(filesize);
    file.read(buffer.data(), filesize);
    file.close();

    auto game_info = GameInfo {
        mGamePath.c_str(),
        static_cast<void*>(buffer.data()),
        static_cast<size_t>(filesize),
        nullptr
    };
    mRetroLoadFn(&game_info);
}

void Core::LoadSave() {
    std::ifstream file(mSavePath, std::ios::binary);
    if (file.fail()) {
        // No save file present
        return;
    }
    const auto save_size = mRetroGetMemorySizeFn(RETRO_MEMORY_SAVE_RAM);
    char* save_ptr = static_cast<char *>(mRetroGetMemoryDataFn(RETRO_MEMORY_SAVE_RAM));
    if (save_size == 0 || !save_ptr) {
        return;
    }
    file.read(save_ptr, save_size);
    file.close();
}

void Core::RunGame() {
    mRetroRunFn();
}

void Core::WriteSave() {
    const auto save_size = mRetroGetMemorySizeFn(RETRO_MEMORY_SAVE_RAM);
    char* save_ptr = static_cast<char *>(mRetroGetMemoryDataFn(RETRO_MEMORY_SAVE_RAM));
    if (save_size == 0) {
        return;
    }
    std::ofstream file(mSavePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open save file\n";
        return;
    }
    file.write(save_ptr, save_size);
    file.close();
}
