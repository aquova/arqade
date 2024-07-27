#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <filesystem>
#include <unordered_map>
#include <vector>

#include <Qt>

#include "ControllerData.hpp"

typedef std::unordered_map<Qt::Key, JoypadButton> ControllerMapType;

struct TabData {
    int mId;
    std::string mTitle;
    std::string mCore;
    std::string mPath;
};

void DbDeleteTab(const int aIdx);
std::vector<TabData> DbGetAllTabs();
ControllerType DbGetControllerType(const int aTab);
std::vector<Qt::Key> DbGetKeybind(const int aTab, const ControllerType aType);
ControllerMapType DbGetMap(const int aTab);
int DbGetNextId();
std::filesystem::path GetConfigDir();
std::filesystem::path GetDatabasePath();
void DbInitialize();
void DbUpdateKeybind(const int aTab, const ControllerData aData);
void DbUpdateTab(const TabData aData);

#endif
