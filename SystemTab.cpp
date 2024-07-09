#include "SystemTab.hpp"
#include "ui_SystemTab.h"

#include <filesystem>

#include <QFileDialog>
#include <QPushButton>

#include "Utils.hpp"

SystemTab::SystemTab(QWidget* aParent, const std::string aTitle): QWidget(aParent), mTab(new Ui::SystemTab) {
    mTab->setupUi(this);
    mTab->nameText->setPlaceholderText(aTitle.c_str());
    UpdateGames();

    connect(mTab->gamePathButton, &QPushButton::pressed, this, &SystemTab::HandleGamePathButtonPressed);
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

void SystemTab::HandleGamePathButtonPressed() {
    const auto dir_path = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),"/home", QFileDialog::ShowDirsOnly);
    if (dir_path != nullptr) {
        // NOTE: For now, we'll just support one path, but use a vector to leave option to support more in future
        mPaths.clear();
        mPaths.push_back(dir_path.toStdString());
        mTab->gamePathText->setText(dir_path);
        UpdateGames();
    }
}

void SystemTab::HandleRefreshButtonPressed() {
    // If the user has typed anything into the text field, grab that as our (potential) path
    // Otherwise, we'll keep the paths as is and just refresh the list
    const auto dir_path = mTab->gamePathText->text();
    if (dir_path != "") {
        mPaths.clear();
        mPaths.push_back(dir_path.toStdString());
    }
    UpdateGames();
}

void SystemTab::HandleUpdateTitleButtonPressed() {
    const auto title = mTab->nameText->text();
    emit UpdateTitle(title);
}

void SystemTab::PopulateCores(const std::vector<std::string> &aCores, const int aIdx) {
    for (const auto& core : aCores) {
        mTab->coreBox->addItem(core.c_str());
    }
    mTab->coreBox->setCurrentIndex(aIdx);
}

void SystemTab::UpdateGames() {
    static constexpr auto SAVE_EXT = ".sav";

    std::vector<std::string> games;
    for (const auto& path : mPaths) {
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (file.path().extension() != SAVE_EXT) {
                games.push_back(file.path().string());
            }
        }
    }
    std::sort(games.begin(), games.end(), AlphabetCompare);
    for (const auto& game : games) {
        const auto qstr = QString(game.c_str());
        mTab->gameListWidget->addItem(qstr);
    }
    mTab->gameListWidget->setCurrentRow(0);
}
