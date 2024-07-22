#include "Database.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <QString>
#include <QVariant>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "Utils.hpp"

// TODO: I would love a helper function that handles DB access
// However, it seems the database must be open prior to preparing the query, which makes things messy

static std::vector<Qt::Key> DeserializeController(const std::string aInput, const int aLen);
static std::string SerializeController(const std::vector<Qt::Key> aInput);

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

ControllerType DbGetControllerType(const int aTab) {
    ControllerType type{ControllerType::NES_GB};

    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return type;
    }
    QSqlQuery query;
    query.prepare("SELECT controller FROM tabs WHERE id = :id");
    query.bindValue(":id", QVariant(aTab));
    query.exec();

    if (query.next()) {
        type = static_cast<ControllerType>(query.value(0).toInt());
    }
    return type;
}

ControllerMapType DbGetMap(const int aTab) {
    ControllerMapType map;
    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return map;
    }

    QSqlQuery query;
    query.prepare("SELECT controller, keys FROM tabs WHERE id = :id");
    query.bindValue(":id", QVariant(aTab));
    query.exec();
    if (query.next()) {
        const auto controller_idx = query.value(0).toInt();
        const auto buttons = ControllerTable[controller_idx].mButtons;
        const auto raw_keybinds = query.value(1).toString().toStdString();
        const auto vec = DeserializeController(raw_keybinds, buttons.size());

        for (size_t i = 0; i < buttons.size(); i++) {
            map[vec[i]] = buttons[i];
        }
    }

    db.close();
    return map;
}

std::vector<Qt::Key> DbGetKeybind(const int aTab, const ControllerType aType) {
    const auto type_int = static_cast<int>(aType);
    std::vector<Qt::Key> vec(ControllerTable[type_int].mButtons.size(), Qt::Key_unknown);

    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return vec;
    }

    QSqlQuery query;
    query.prepare("SELECT keys FROM tabs WHERE id = :id AND controller = :controller");
    query.bindValue(":id", QVariant(aTab));
    query.bindValue(":controller", QVariant(type_int));
    query.exec();
    if (query.next()) {
        const auto raw_keybinds = query.value(0).toString().toStdString();
        vec = DeserializeController(raw_keybinds, ControllerTable[type_int].mButtons.size());
    }

    db.close();
    return vec;
}

static std::vector<Qt::Key> DeserializeController(const std::string aInput, const int aLen) {
    std::vector<Qt::Key> vec(aLen, Qt::Key_unknown);

    if (aInput.empty()) {
        return vec;
    }

    std::string tmp;
    std::stringstream stream(aInput);
    std::vector<std::string> words;
    while (getline(stream, tmp, ',')) {
        words.push_back(tmp);
    }

    for (size_t i = 0; i < words.size(); i++) {
        vec[i] = static_cast<Qt::Key>(std::stoi(words[i]));
    }

    return vec;
}

std::filesystem::path GetConfigDir() {
    // TODO: Paths could probably be static
    static constexpr auto CONFIG_FOLDER = "arqade";

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
    query.prepare("CREATE TABLE IF NOT EXISTS tabs (id INTEGER PRIMARY KEY, name TEXT, core TEXT, path TEXT, controller INTEGER, keys TEXT)");
    query.exec();

    db.commit();
    db.close();
}

void DbUpdateKeybind(const int aTab, const ControllerData aData) {
    const auto db_path = GetDatabasePath();
    auto db = QSqlDatabase::database();
    if (!db.open()) {
        std::cerr << "Unable to open database " << db.lastError().text().toStdString() << std::endl;
        return;
    }

    // Attempt to insert an empty row if needed, so the UPDATE statement will work correctly later
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO tabs (id) VALUES (:id)");
    query.bindValue(":id", QVariant(aTab));
    query.exec();
    query.clear();

    const auto serialized = SerializeController(aData.mButtons);
    query.prepare("UPDATE tabs SET controller = :controller, keys = :keys WHERE id = :id");
    query.bindValue(":id", QVariant(aTab));
    query.bindValue(":controller", QVariant(static_cast<int>(aData.mType)));
    query.bindValue(":keys", serialized.c_str());
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
    // Attempt to insert an empty row if needed, so the UPDATE statement will work correctly later
    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO tabs (id) VALUES (:id)");
    query.bindValue(":id", QVariant(aData.mId));
    query.exec();
    query.clear();

    query.prepare("UPDATE tabs SET name = :name, core = :core, path = :path WHERE id = :id");
    query.bindValue(":id", QVariant(aData.mId));
    query.bindValue(":name", aData.mTitle.c_str());
    query.bindValue(":core", aData.mCore.c_str());
    query.bindValue(":path", aData.mPath.c_str());
    query.exec();

    db.commit();
    db.close();
}

static std::string SerializeController(const std::vector<Qt::Key> aInput) {
    std::string output;
    for (const auto& key : aInput) {
        output = Join(output, std::to_string(key), ",");
    }
    return output;
}
