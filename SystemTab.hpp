#ifndef SYSTEMTAB_HPP
#define SYSTEMTAB_HPP

#include <vector>

#include <QListWidgetItem>
#include <QString>
#include <QWidget>

#include "Database.hpp"

QT_BEGIN_NAMESPACE namespace Ui {
class SystemTab;
}
QT_END_NAMESPACE

class SystemTab :  public QWidget {
    Q_OBJECT

    public:
        SystemTab(QWidget *aParent, const TabData aData, const std::vector<std::string> aCores);
        ~SystemTab();
        int GetId();
        std::string GetSelectedCore();
        std::string GetSelectedGame();
        std::string GetTabFolder();

    signals:
        void RunSelected();
        void UpdateTitle(QString aTitle);

    private slots:
        void HandleControlsButtonPressed();
        void HandleCoreChanged(const QString &aText);
        void HandleGamePathButtonPressed();
        void HandleItemDoubleClicked();
        void HandlePopupClose();
        void HandleRefreshButtonPressed();
        void HandleUpdateTitle();

    private:
        void LoadData();
        void PopulateCores(const std::vector<std::string> &aCores);
        void UpdateGames();

        TabData mData;
        bool mPopupShowing{false};
        Ui::SystemTab* mTab;
};

#endif
