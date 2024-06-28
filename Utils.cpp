#include "Utils.hpp"

#include "CoreData.hpp"

std::vector<uint8_t> ConvertRGB1555ToRGB888(const std::vector<uint8_t> &aVector) {
    std::vector<uint8_t> output;
    const auto bytes_per_pixel = CoreData::getInstance().GetBytesPerPixel();

    for (int i = 0; i < aVector.size(); i += bytes_per_pixel) {
        const auto first = aVector.at(i);
        const auto second = aVector.at(i + 1);

        const auto red = (first & 0b01111100) >> 2;
        const auto green = ((first & 0b11) << 3) | ((second & 0b11100000) >> 5);
        const auto blue = second & 0b00011111;

        const auto scaled_red = static_cast<uint8_t>(red * 255.0 / 31.0);
        const auto scaled_green = static_cast<uint8_t>(green * 255.0 / 31.0);
        const auto scaled_blue = static_cast<uint8_t>(blue * 255.0 / 31.0);

        output.push_back(scaled_red);
        output.push_back(scaled_green);
        output.push_back(scaled_blue);
    }

    return output;
}

std::vector<uint8_t> ConvertRGB565ToRGB888(const std::vector<uint8_t> &aVector) {
    std::vector<uint8_t> output;
    const auto bytes_per_pixel = CoreData::getInstance().GetBytesPerPixel();

    for (int i = 0; i < aVector.size(); i += bytes_per_pixel) {
        // I assume this is an endian thing
        const auto second = aVector.at(i);
        const auto first = aVector.at(i + 1);

        const auto red = (first & 0b11111000) >> 3;
        const auto green = ((first & 0b111) << 3) | ((second & 0b11100000) >> 5);
        const auto blue = second & 0b00011111;

        const auto scaled_red = static_cast<uint8_t>(red * 255.0 / 31.0);
        const auto scaled_green = static_cast<uint8_t>(green * 255.0 / 63.0);
        const auto scaled_blue = static_cast<uint8_t>(blue * 255.0 / 31.0);

        output.push_back(scaled_red);
        output.push_back(scaled_green);
        output.push_back(scaled_blue);
    }

    return output;
}

std::vector<uint8_t> ConvertRGB8888ToRGB888(const std::vector<uint8_t> &aVector) {
    std::vector<uint8_t> output;
    const auto bytes_per_pixel = CoreData::getInstance().GetBytesPerPixel();

    for (int i = 0; i < aVector.size(); i += bytes_per_pixel) {
        // Despite what XRGB8888 might imply, it's actually XBGR8888
        output.push_back(aVector.at(i + 2));
        output.push_back(aVector.at(i + 1));
        output.push_back(aVector.at(i + 0));
    }

    return output;
}
