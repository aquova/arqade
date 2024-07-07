#include "Window.hpp"
#include "qobject.h"
#include "ui_Window.h"

#include <filesystem>
#include <QFileDialog>
#include <QString>
#include <iostream>

#include "Arqade.hpp"
#include "Database.hpp"
#include "SystemTab.hpp"

ArqadeWindow::ArqadeWindow(QWidget *parent): QMainWindow(parent), mUi(new Ui::ArqadeWindow) {
    mUi->setupUi(this);
    // CreateConfig();
    PopulateTabs();

    connect(mUi->startButton, &QPushButton::pressed, this, &ArqadeWindow::HandleRunButtonPressed);
}

ArqadeWindow::~ArqadeWindow() {
    delete mUi;
}

void ArqadeWindow::AddTab() {

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

void ArqadeWindow::HandleRunButtonPressed() {
    QWidget* current_widget = mUi->tabWidget->currentWidget();
    SystemTab* current_tab = qobject_cast<SystemTab*>(current_widget);
    const auto romPath = current_tab->GetSelectedGame();
    const auto corePath = current_tab->GetSelectedCore();
    RunEmu(romPath, corePath);
}

void ArqadeWindow::PopulateTabs() {
    // TODO: Read from DB
    auto tab = new SystemTab;
    mUi->tabWidget->insertTab(0, tab, tr("Test"));
    mUi->tabWidget->setCurrentIndex(0);
}
