#ifndef CONTROLLERDATA_HPP
#define CONTROLLERDATA_HPP

#include <string>
#include <vector>

#include <Qt>

#include "libretro.hpp"

enum class ControllerType {
    NES_GB,
    SMS_GG,
    SNES_DS,
    GBA,
    MD_3BTN,
    MD_6BTN,
    SATURN,
    DC,
    DUALSHOCK,
    XBOX,
    NINTENDO,
};

struct ControllerLayout {
    ControllerType mControllerType;
    std::string mName;
    std::vector<JoypadButton> mButtons;
    std::vector<std::string> mButtonNames;
};

struct ControllerData {
    ControllerType mType;
    std::vector<Qt::Key> mButtons;
};

const ControllerLayout ControllerTable[] = {
    {
        ControllerType::NES_GB, "NES/GB",
        {JoypadButton::EAST, JoypadButton::SOUTH, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::START, JoypadButton::SELECT},
        {"A", "B", "Up", "Down", "Left", "Right", "Start", "Select"},
    },
    {
        ControllerType::SMS_GG, "SMS/GG",
        {JoypadButton::EAST, JoypadButton::SOUTH, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::START},
        {"1", "2", "Up", "Down", "Left", "Right", "Start"},
    },
    {
        ControllerType::SNES_DS, "SNES/DS",
        {JoypadButton::EAST, JoypadButton::SOUTH, JoypadButton::NORTH, JoypadButton::WEST, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::L, JoypadButton::R, JoypadButton::START, JoypadButton::SELECT},
        {"A", "B", "X", "Y", "Up", "Down", "Left", "Right", "L", "R", "Start", "Select"},
    },
    {
        ControllerType::GBA, "GBA",
        {JoypadButton::EAST, JoypadButton::SOUTH, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::L, JoypadButton::R, JoypadButton::START, JoypadButton::SELECT},
        {"A", "B", "Up", "Down", "Left", "Right", "L", "R", "Start", "Select"},
    },
    {
        ControllerType::MD_3BTN, "Sega Genesis 3-Button",
        {JoypadButton::WEST, JoypadButton::SOUTH, JoypadButton::EAST, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::START, JoypadButton::SELECT},
        {"A", "B", "C", "Up", "Down", "Left", "Right", "Start", "Mode"},
    },
    {
        ControllerType::MD_6BTN, "Sega Genesis 6-Button",
        {JoypadButton::WEST, JoypadButton::SOUTH, JoypadButton::EAST, JoypadButton::L, JoypadButton::NORTH, JoypadButton::R, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::START, JoypadButton::SELECT},
        {"A", "B", "C", "X", "Y", "Z", "Up", "Down", "Left", "Right", "Start", "Mode"},
    },
};


#endif
