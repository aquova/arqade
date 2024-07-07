#include <filesystem>
#include <QFileDialog>
#include <QString>

#include "Arqade.hpp"
#include "Window.hpp"
#include "ui_Window.h"

static const auto LIBRETRO_CORE_PATH = "/usr/lib/libretro/";

ArqadeWindow::ArqadeWindow(QWidget *parent): QMainWindow(parent), mUi(new Ui::ArqadeWindow) {
    mUi->setupUi(this);
    PopulateCores();

    connect(mUi->romButton, &QPushButton::pressed, this, &ArqadeWindow::HandleGameButtonPressed);
    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::HandleRunButtonPressed);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
}


void ArqadeWindow::HandleGameButtonPressed() {
    mGameSelected = false;
    const auto romPath = QFileDialog::getOpenFileName(this, tr("Open ROM"), "~", tr("Game ROM (*)"));
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
