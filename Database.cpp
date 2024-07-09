#include "Database.hpp"

#include <cstdlib>
#include <iostream>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

static constexpr auto CONFIG_FOLDER = "arqade";
static constexpr auto DATABASE_NAME = "arqade.db";

std::filesystem::path GetConfigDir() {
    // TODO: This needs to be different for Windows
    const auto home = std::getenv("HOME");
    const auto default_linux = std::filesystem::path(home) / ".config";

    const auto xdg_config = std::getenv("XDG_DATA_HOME");
    const auto config = xdg_config == nullptr ? default_linux : std::filesystem::path(xdg_config);
    return config / CONFIG_FOLDER;
}

std::filesystem::path GetDatabasePath() {
    const auto config_dir = GetConfigDir();
    return config_dir / DATABASE_NAME;
}

void InitializeDb() {
    const auto db_path = GetDatabasePath();
    std::cout << "Initializing database " << db_path << std::endl;
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path.c_str());
    if (!db.open()) {
        std::cerr << "Unable to open database\n";
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS tabs (id INTEGER PRIMARY KEY, name TEXT, core TEXT, path TEXT)");
    db.commit();
    db.close();
}
