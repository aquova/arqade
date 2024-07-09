#include "Database.hpp"

#include <cstdlib>
#include <iostream>

#include <QString>
#include <QVariant>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

// TODO: I would love a helper function that handles DB access
// However, it seems the database must be open prior to preparing the query, which makes things messy

void DbDeleteTab(const int aIdx) {
    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM tabs WHERE id = :id");
    query.bindValue(":id", QVariant(aIdx));
    query.exec();

    db.commit();
    db.close();
}

std::vector<TabData> DbGetAllTabs() {
    std::vector<TabData> tab_vec;

    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return tab_vec;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM tabs");
    query.exec();

    while (query.next()) {
        const auto new_tab = TabData {
            query.value(0).toInt(),
            query.value(1).toString().toStdString(),
            query.value(2).toString().toStdString(),
            query.value(3).toString().toStdString()
        };
        tab_vec.push_back(new_tab);
    }
    db.close();
    return tab_vec;
}

std::filesystem::path GetConfigDir() {
    static constexpr auto CONFIG_FOLDER = "arqade";

    // TODO: This needs to be different for Windows
    const auto home = std::getenv("HOME");
    const auto default_linux = std::filesystem::path(home) / ".config";

    const auto xdg_config = std::getenv("XDG_DATA_HOME");
    const auto config = xdg_config == nullptr ? default_linux : std::filesystem::path(xdg_config);
    return config / CONFIG_FOLDER;
}

std::filesystem::path GetDatabasePath() {
    static constexpr auto DATABASE_NAME = "arqade.db";

    const auto config_dir = GetConfigDir();
    return config_dir / DATABASE_NAME;
}

void DbInitialize() {
    // Initial setup
    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path.c_str());
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return;
    }

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS tabs (id INTEGER PRIMARY KEY, name TEXT, core TEXT, path TEXT)");
    query.exec();

    db.commit();
    db.close();
}

void DbUpdateTab(const TabData aData) {
    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO tabs (id, name, core, path) VALUES (:id, :name, :core, :path)");
    query.bindValue(":id", QVariant(aData.mId));
    query.bindValue(":name", aData.mTitle.c_str());
    query.bindValue(":core", aData.mCore.c_str());
    query.bindValue(":path", aData.mPath.c_str());
    query.exec();

    db.commit();
    db.close();
}
