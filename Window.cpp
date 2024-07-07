#include <filesystem>
#include <QFileDialog>
#include <QString>
#include <iostream>

#include "Arqade.hpp"
#include "Database.hpp"
#include "Window.hpp"
#include "ui_Window.h"

static constexpr auto LIBRETRO_CORE_PATH = "/usr/lib/libretro/";

ArqadeWindow::ArqadeWindow(QWidget *parent): QMainWindow(parent), mUi(new Ui::ArqadeWindow) {
    mUi->setupUi(this);
    CreateConfig();
    PopulateCores();

    connect(mUi->romButton, &QPushButton::pressed, this, &ArqadeWindow::HandleGameButtonPressed);
    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::HandleRunButtonPressed);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
}

void ArqadeWindow::CreateConfig() {
    const auto db_dir = GetConfigDir();
    if (!std::filesystem::exists(db_dir)) {
        std::cout << "Creating config directory " << db_dir << std::endl;
        std::filesystem::create_directory(db_dir);
    }

    const auto db_path = GetDatabasePath();
    if (!std::filesystem::exists(db_path)) {
        InitializeDb();
    }
}

void ArqadeWindow::HandleGameButtonPressed() {
    mGameSelected = false;
    const auto romPath = QFileDialog::getOpenFileName(this, tr("Open ROM"), QDir::homePath(), tr("Game ROM (*)"));
    if (romPath != nullptr) {
        mUi->romPathEdit->setText(romPath);
        mGameSelected = true;
    }
}

void ArqadeWindow::HandleRunButtonPressed() {
    if (mGameSelected) {
        const auto romPath = mUi->romPathEdit->text();
        const auto corePath = mUi->coreBox->currentText();
        RunEmu(romPath.toStdString(), corePath.toStdString());
    }
}

void ArqadeWindow::PopulateCores() {
    for (const auto& core : std::filesystem::directory_iterator(LIBRETRO_CORE_PATH)) {
        const auto qstr = QString(core.path().c_str());
        mUi->coreBox->addItem(qstr);
    }
}
