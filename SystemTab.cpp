#include "SystemTab.hpp"
#include "ui_SystemTab.h"

#include <filesystem>

#include "Utils.hpp"

static constexpr auto LIBRETRO_CORE_PATH = "/usr/lib/libretro/";

SystemTab::SystemTab(QWidget* aParent): QWidget(aParent), mTab(new Ui::SystemTab) {
    static constexpr auto DEBUG_PATH = "/beta/roms/gba";

    mTab->setupUi(this);
    PopulateCores();
    mPaths.push_back(DEBUG_PATH);
    UpdateGames();
    PopulateList();
}

SystemTab::~SystemTab() {
    delete mTab;
}

std::string SystemTab::GetSelectedCore() {
    return mTab->coreBox->currentText().toStdString();
}

std::string SystemTab::GetSelectedGame() {
    return mTab->gameListWidget->currentItem()->text().toStdString();
}

void SystemTab::PopulateCores() {
    for (const auto& core : std::filesystem::directory_iterator(LIBRETRO_CORE_PATH)) {
        const auto qstr = QString(core.path().c_str());
        mTab->coreBox->addItem(qstr);
    }
}

void SystemTab::PopulateList() {
    for (const auto& game : mGames) {
        const auto qstr = QString(game.c_str());
        mTab->gameListWidget->addItem(qstr);
    }
    mTab->gameListWidget->setCurrentRow(0);
}

void SystemTab::UpdateGames() {
    mGames.clear();
    for (const auto& path : mPaths) {
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            mGames.push_back(file.path().string());
        }
    }
    std::sort(mGames.begin(), mGames.end(), AlphabetCompare);
}
