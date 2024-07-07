#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <filesystem>

std::filesystem::path GetConfigDir();
std::filesystem::path GetDatabasePath();
void InitializeDb();

#endif
