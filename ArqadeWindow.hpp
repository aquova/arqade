#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QThread>

#include "Arqade.hpp"
#include "Database.hpp"

QT_BEGIN_NAMESPACE namespace Ui {
class ArqadeWindow;
}
QT_END_NAMESPACE

class ArqadeWindow : public QMainWindow {
    Q_OBJECT

    public:
        ArqadeWindow(QWidget *parent = nullptr);
        ~ArqadeWindow();

    signals:
        void RunEmu(const QString &aRomPath, const QString &aCorePath, const ControllerMapType aBindings);

    private slots:
        void HandleAddTabPressed();
        void HandleDeleteTabPressed();
        void HandleUpdateTitle(QString aTitle);
        void RunSelectedGame();

    private:
        void AddTab(const TabData aData);
        void CreateConfig();
        void CreateEmptyTab();
        void PopulateTabs();

        Arqade* mEmulator;
        std::vector<std::string> mCores;
        QThread* mEmuThread;
        Ui::ArqadeWindow *mUi;
};
#endif
