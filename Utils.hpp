#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#include <Qt>
#include <SDL_keycode.h>

bool AlphabetCompare(std::string aLhs, std::string aRhs);
std::string Join(std::string aLhs, std::string aRhs, std::string aDelim);
Qt::Key SDL2Qt(const SDL_Keycode aKey);

#endif
