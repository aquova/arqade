#include "Window.hpp"
#include "ui_Window.h"

#include <filesystem>
#include <iostream>
#include <vector>

#include <QString>
#include "qobject.h"

#include "Arqade.hpp"
#include "Database.hpp"
#include "SystemTab.hpp"

static constexpr auto LIBRETRO_CORE_PATH = "/usr/lib/libretro/";

ArqadeWindow::ArqadeWindow(QWidget *parent): QMainWindow(parent), mUi(new Ui::ArqadeWindow) {
    mUi->setupUi(this);
    for (const auto& core : std::filesystem::directory_iterator(LIBRETRO_CORE_PATH)) {
        mCores.push_back(core.path().string());
    }
    // CreateConfig();
    PopulateTabs();

    connect(mUi->addTabButton, &QPushButton::pressed, this, &ArqadeWindow::HandleAddTabPressed);
    connect(mUi->deleteTabButton, &QPushButton::pressed, this, &ArqadeWindow::HandleDeleteTabPressed);
    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::HandleRunButtonPressed);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
}

void ArqadeWindow::AddTab(const std::string aTitle, const int aIdx) {
    auto tab = new SystemTab(nullptr, aTitle);
    tab->PopulateCores(mCores, aIdx);
    mUi->tabWidget->addTab(tab, tr(aTitle.c_str()));
    connect(tab, &SystemTab::UpdateTitle, this, &ArqadeWindow::HandleUpdateTitle);
    mUi->tabWidget->setCurrentIndex(0);
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

void ArqadeWindow::CreateDefaultTabs() {
    for (size_t i = 0; i < mCores.size(); i++) {
        AddTab(std::to_string(i + 1), i);
    }
}

void ArqadeWindow::HandleAddTabPressed() {
    AddTab("New", 0);
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
    CreateDefaultTabs();
}
