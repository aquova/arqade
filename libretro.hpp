// Redefinitions of some items defined in libretro.h
// https://github.com/libretro/libretro-common/blob/master/include/libretro.h
// TODO: I don't know if there's a better way to utilize their interface than recreating the consts/structs we need

#ifndef LIBRETRO_HPP
#define LIBRETRO_HPP

#include <climits>
#include <cstddef>

constexpr auto RETRO_ENV_GET_CAN_DUPE = 3;
constexpr auto RETRO_ENV_SET_PIXEL_FORMAT = 10;

constexpr auto RETRO_MEMORY_SAVE_RAM = 0;

// Joypad is laid out like SNES controller
enum JoypadButton {
    B,
    Y,
    SELECT,
    START,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    A,
    X,
    L,
    R,
    L2,
    R2,
    L3,
    R3,
    COUNT,
};

enum PixelFormat {
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

#endif
