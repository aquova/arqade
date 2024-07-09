#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <filesystem>
#include <vector>

struct TabData {
    int mId;
    std::string mTitle;
    std::string mCore;
    std::string mPath;
};

void DbDeleteTab(const int aIdx);
std::vector<TabData> DbGetAllTabs();
std::filesystem::path GetConfigDir();
std::filesystem::path GetDatabasePath();
void DbInitialize();
void DbUpdateTab(const TabData aData);

#endif
