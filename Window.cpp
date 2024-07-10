#include "Window.hpp"
#include "ui_Window.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include <QString>
#include "qobject.h"

#include "Arqade.hpp"
#include "SystemTab.hpp"

static constexpr auto LIBRETRO_CORE_PATH = "/usr/lib/libretro/";

ArqadeWindow::ArqadeWindow(QWidget *parent): QMainWindow(parent), mUi(new Ui::ArqadeWindow) {
    mUi->setupUi(this);
    mEmulator = new Arqade;
    mEmuThread = new QThread;
    mEmulator->moveToThread(mEmuThread);
    mEmuThread->start();

    for (const auto& core : std::filesystem::directory_iterator(LIBRETRO_CORE_PATH)) {
        mCores.push_back(core.path().string());
    }
    CreateConfig();
    PopulateTabs();

    connect(this, &ArqadeWindow::RunEmu, mEmulator, &Arqade::RunEmu);
    connect(mUi->addTabButton, &QPushButton::pressed, this, &ArqadeWindow::HandleAddTabPressed);
    connect(mUi->deleteTabButton, &QPushButton::pressed, this, &ArqadeWindow::HandleDeleteTabPressed);
    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::RunSelectedGame);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
    mEmuThread->quit();
    mEmuThread->wait();
    delete mEmuThread;
    delete mEmulator;
}

void ArqadeWindow::AddTab(const TabData aData) {
    auto tab = new SystemTab(nullptr, aData, mCores);
    mUi->tabWidget->addTab(tab, aData.mTitle.c_str());
    connect(tab, &SystemTab::RunSelected, this, &ArqadeWindow::RunSelectedGame);
    connect(tab, &SystemTab::UpdateTitle, this, &ArqadeWindow::HandleUpdateTitle);
    DbUpdateTab(aData);
}

void ArqadeWindow::CreateConfig() {
    const auto db_dir = GetConfigDir();
    if (!std::filesystem::exists(db_dir)) {
        std::cout << "Creating config directory " << db_dir << std::endl;
        std::filesystem::create_directory(db_dir);
    }
    DbInitialize();
}

void ArqadeWindow::CreateEmptyTab() {
    const auto new_tab = TabData {
        mUi->tabWidget->count(),
        "NEW",
        mCores[0],
        ""
    };
    AddTab(new_tab);
}

void ArqadeWindow::HandleAddTabPressed() {
    CreateEmptyTab();
}

void ArqadeWindow::HandleDeleteTabPressed() {
    mUi->tabWidget->removeTab(mUi->tabWidget->currentIndex());
}

void ArqadeWindow::HandleUpdateTitle(QString aTitle) {
    mUi->tabWidget->setTabText(mUi->tabWidget->currentIndex(), aTitle);
}

void ArqadeWindow::PopulateTabs() {
    const auto tabs = DbGetAllTabs();
    if (tabs.size() > 0) {
        for (const auto& tab : tabs) {
            AddTab(tab);
        }
    } else {
        CreateEmptyTab();
    }
}

void ArqadeWindow::RunSelectedGame() {
    QWidget* current_widget = mUi->tabWidget->currentWidget();
    SystemTab* current_tab = qobject_cast<SystemTab*>(current_widget);
    const auto romPath = current_tab->GetSelectedGame();
    const auto corePath = current_tab->GetSelectedCore();
    emit RunEmu(romPath.c_str(), corePath.c_str());
}
