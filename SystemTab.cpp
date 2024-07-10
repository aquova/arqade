#include "SystemTab.hpp"
#include "ui_SystemTab.h"

#include <filesystem>

#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>

#include "Utils.hpp"

SystemTab::SystemTab(QWidget* aParent, const TabData aData, const std::vector<std::string> aCores): QWidget(aParent), mData(aData), mTab(new Ui::SystemTab) {
    mTab->setupUi(this);
    PopulateCores(aCores);
    LoadData();

    connect(mTab->coreBox, &QComboBox::currentTextChanged, this, &SystemTab::HandleCoreChanged);
    connect(mTab->gamePathButton, &QPushButton::pressed, this, &SystemTab::HandleGamePathButtonPressed);
    connect(mTab->refreshButton, &QPushButton::pressed, this, &SystemTab::HandleRefreshButtonPressed);
    connect(mTab->nameText, &QLineEdit::returnPressed, this, &SystemTab::HandleUpdateTitle);
    connect(mTab->gameListWidget, &QListWidget::itemDoubleClicked, this, &SystemTab::HandleItemDoubleClicked);
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

void SystemTab::HandleCoreChanged(const QString &aText) {
    mData.mCore = aText.toStdString();
    DbUpdateTab(mData);
}

void SystemTab::HandleGamePathButtonPressed() {
    const auto dir_path = QFileDialog::getExistingDirectory(this, tr("Choose Folder"), "/home", QFileDialog::ShowDirsOnly);
    if (dir_path != nullptr) {
        mData.mPath = dir_path.toStdString();
        mTab->gamePathText->setText(dir_path);
        DbUpdateTab(mData);
        UpdateGames();
    }
}

void SystemTab::HandleItemDoubleClicked() {
    emit RunSelected();
}

void SystemTab::HandleRefreshButtonPressed() {
    UpdateGames();
}

void SystemTab::HandleUpdateTitle() {
    const auto title = mTab->nameText->text();
    mData.mTitle = title.toStdString();
    emit UpdateTitle(title);
    DbUpdateTab(mData);
}

void SystemTab::LoadData() {
    mTab->nameText->setText(mData.mTitle.c_str());
    mTab->gamePathText->setText(mData.mPath.c_str());
    // TODO: Need to see what happens if core is deleted and no longer in list
    mTab->coreBox->setCurrentText(mData.mCore.c_str());
    UpdateGames();
}

void SystemTab::PopulateCores(const std::vector<std::string> &aCores) {
    for (const auto& core : aCores) {
        mTab->coreBox->addItem(core.c_str());
    }
}

void SystemTab::UpdateGames() {
    static constexpr auto SAVE_EXT = ".sav";

    if (mData.mPath.empty()) {
        return;
    }

    std::vector<std::string> games;
    for (const auto& file : std::filesystem::directory_iterator(mData.mPath)) {
        if (file.path().extension() != SAVE_EXT) {
            games.push_back(file.path().string());
        }
    }
    std::sort(games.begin(), games.end(), AlphabetCompare);
    mTab->gameListWidget->clear();
    for (const auto& game : games) {
        const auto qstr = QString(game.c_str());
        mTab->gameListWidget->addItem(qstr);
    }
    mTab->gameListWidget->setCurrentRow(0);
}
