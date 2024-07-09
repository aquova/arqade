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
        std::string GetSelectedCore();
        std::string GetSelectedGame();

    signals:
        void UpdateTitle(QString aTitle);

    private slots:
        void HandleCoreChanged(const QString &aText);
        void HandleGamePathButtonPressed();
        void HandleRefreshButtonPressed();
        void HandleUpdateTitleButtonPressed();

    private:
        void LoadData();
        void PopulateCores(const std::vector<std::string> &aCores);
        void UpdateGames();

        TabData mData;
        Ui::SystemTab* mTab;
};

#endif
