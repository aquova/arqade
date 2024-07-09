#include "SystemTab.hpp"
#include "ui_SystemTab.h"

#include <filesystem>
#include <QPushButton>

#include "Utils.hpp"

SystemTab::SystemTab(QWidget* aParent, const std::string aTitle): QWidget(aParent), mTab(new Ui::SystemTab) {
    // static constexpr auto DEBUG_PATH = "/home/aquova/games/roms/gba";
    static constexpr auto DEBUG_PATH = "/beta/roms/gba";

    mTab->setupUi(this);
    mTab->nameLineEdit->setPlaceholderText(aTitle.c_str());
    mPaths.push_back(DEBUG_PATH);
    UpdateGames();
    PopulateList();

    connect(mTab->refreshButton, &QPushButton::pressed, this, &SystemTab::HandleRefreshButtonPressed);
    connect(mTab->nameUpdateButton, &QPushButton::pressed, this, &SystemTab::HandleUpdateTitleButtonPressed);
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

void SystemTab::HandleRefreshButtonPressed() {
    UpdateGames();
}

void SystemTab::HandleUpdateTitleButtonPressed() {
    const auto title = mTab->nameLineEdit->text();
    emit updateTitle(title);
}

void SystemTab::PopulateCores(const std::vector<std::string> &aCores, const int aIdx) {
    for (const auto& core : aCores) {
        mTab->coreBox->addItem(core.c_str());
    }
    mTab->coreBox->setCurrentIndex(aIdx);
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
