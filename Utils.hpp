#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <Qt>
#include <SDL_keycode.h>

bool AlphabetCompare(std::string aLhs, std::string aRhs);
std::vector<uint8_t> ConvertRGB1555ToRGB888(const std::vector<uint8_t> &aVector);
std::vector<uint8_t> ConvertRGB565ToRGB888(const std::vector<uint8_t> &aVector);
std::vector<uint8_t> ConvertRGB8888ToRGB888(const std::vector<uint8_t> &aVector);
std::string Join(std::string aLhs, std::string aRhs, std::string aDelim);
Qt::Key SDL2Qt(const SDL_Keycode aKey);

#endif
