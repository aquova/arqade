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
    for (const auto& core : std::filesystem::directory_iterator(LIBRETRO_CORE_PATH)) {
        mCores.push_back(core.path().string());
    }
    CreateConfig();
    PopulateTabs();

    connect(mUi->addTabButton, &QPushButton::pressed, this, &ArqadeWindow::HandleAddTabPressed);
    connect(mUi->deleteTabButton, &QPushButton::pressed, this, &ArqadeWindow::HandleDeleteTabPressed);
    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::HandleRunButtonPressed);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
}

void ArqadeWindow::AddTab(const TabData aData) {
    auto tab = new SystemTab(nullptr, aData, mCores);
    mUi->tabWidget->addTab(tab, aData.mTitle.c_str());
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

void ArqadeWindow::HandleRunButtonPressed() {
    QWidget* current_widget = mUi->tabWidget->currentWidget();
    SystemTab* current_tab = qobject_cast<SystemTab*>(current_widget);
    const auto romPath = current_tab->GetSelectedGame();
    const auto corePath = current_tab->GetSelectedCore();
    RunEmu(romPath, corePath);
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
