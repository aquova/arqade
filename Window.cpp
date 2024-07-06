#include <QFileDialog>

#include "Arqade.hpp"
#include "Window.h"
#include "ui_Window.h"

ArqadeWindow::ArqadeWindow(QWidget *parent): QMainWindow(parent), mUi(new Ui::ArqadeWindow) {
    mUi->setupUi(this);
    connect(mUi->coreButton, &QPushButton::pressed, this, &ArqadeWindow::HandleCoreButtonPressed);
    connect(mUi->romButton, &QPushButton::pressed, this, &ArqadeWindow::HandleGameButtonPressed);
    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::HandleRunButtonPressed);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
}


void ArqadeWindow::HandleCoreButtonPressed() {
    mCoreSelected = false;
    const auto corePath = QFileDialog::getOpenFileName(this, tr("Open Core"), "/", tr("libretro Core Files (*.so)"));
    if (corePath != nullptr) {
        mUi->corePathEdit->setText(corePath);
        mCoreSelected = true;
    }
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
    if (mCoreSelected && mGameSelected) {
        const auto romPath = mUi->romPathEdit->text();
        const auto corePath = mUi->corePathEdit->text();
        RunEmu(romPath.toStdString(), corePath.toStdString());
    }
}
