// Redefinitions of some items defined in libretro.h
// https://github.com/libretro/libretro-common/blob/master/include/libretro.h
// TODO: I don't know if there's a better way to utilize their interface than recreating the consts/structs we need

#ifndef LIBRETRO_HPP
#define LIBRETRO_HPP

#include <climits>
#include <cstddef>

constexpr auto RETRO_ENV_GET_OVERSCAN = 2;
constexpr auto RETRO_ENV_GET_CAN_DUPE = 3;
constexpr auto RETRO_ENV_SET_PIXEL_FORMAT = 10;
constexpr auto RETRO_ENV_GET_VARIABLE_UPDATE = 17;
constexpr auto RETRO_ENV_SET_GEOMETRY = 37;

constexpr auto RETRO_MEMORY_SAVE_RAM = 0;

enum class JoypadButton {
    SOUTH,
    WEST,
    SELECT,
    START,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    EAST,
    NORTH,
    L,
    R,
    L2,
    R2,
    L3,
    R3,
    COUNT,
};

enum class PixelFormat {
    RGB1555 = 0,
    XRGB8888 = 1,
    RGB565 = 2,
    UNKNOWN = INT_MAX,
};

struct GameInfo {
    const char* mPath;
    const void* mData;
    size_t mSize;
    const char* mMeta;
};

struct GameGeometry {
    unsigned mBaseWidth;
    unsigned mBaseHeight;
    unsigned mMaxWidth;
    unsigned mMaxHeight;
    float mAspectRatio;
};

struct SystemTiming {
    double mFps;
    double mSampleRate;
};

struct AvInfo {
    struct GameGeometry mGeometry;
    struct SystemTiming mTiming;
};

#endif
