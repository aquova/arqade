#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>
#include <vector>

std::vector<uint8_t> ConvertRGB1555ToRGB888(const std::vector<uint8_t> &aVector);
std::vector<uint8_t> ConvertRGB565ToRGB888(const std::vector<uint8_t> &aVector);
std::vector<uint8_t> ConvertRGB8888ToRGB888(const std::vector<uint8_t> &aVector);

#endif
