#ifndef CORE_HPP
#define CORE_HPP

#include <string>
#include <sys/types.h>

#include "libretro.hpp"

typedef bool (*RetroLoadGame)(GameInfo* aGameInfo);
typedef void (*RetroRunType)();
typedef void* (*RetroGetMemoryDataType)(unsigned aId);
typedef size_t (*RetroGetMemorySizeType)(unsigned aId);
typedef void (*RetroGetSystemAvInfoType)(AvInfo* aInfo);

class Core {
    public:
        Core(std::string aFilePath, std::string aLibPath);
        ~Core();
        void RunGame();
        void WriteSave();

    private:
        void InitCore();
        void LoadGame();
        void LoadSave();

        void* mCoreLib{nullptr};
        std::string mGamePath;
        std::string mSavePath;
        std::string mLibPath;

        RetroLoadGame mRetroLoadFn{nullptr};
        RetroRunType mRetroRunFn{nullptr};
        RetroGetSystemAvInfoType mRetroGetSystemAvInfoFn{nullptr};
        RetroGetMemoryDataType mRetroGetMemoryDataFn{nullptr};
        RetroGetMemorySizeType mRetroGetMemorySizeFn{nullptr};
};

#endif
