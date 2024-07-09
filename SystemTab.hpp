#ifndef SYSTEMTAB_HPP
#define SYSTEMTAB_HPP

#include <filesystem>
#include <vector>

#include <QListWidgetItem>
#include <QString>
#include <QWidget>

QT_BEGIN_NAMESPACE namespace Ui {
class SystemTab;
}
QT_END_NAMESPACE

class SystemTab :  public QWidget {
    Q_OBJECT

    public:
        SystemTab(QWidget *aParent = nullptr, const std::string aTitle = "");
        ~SystemTab();
        std::string GetSelectedCore();
        std::string GetSelectedGame();
        void PopulateCores(const std::vector<std::string> &aCores, const int aIdx);

    signals:
        void UpdateTitle(QString aTitle);

    private slots:
        void HandleGamePathButtonPressed();
        void HandleRefreshButtonPressed();
        void HandleUpdateTitleButtonPressed();

    private:
        void UpdateGames();

        std::vector<std::filesystem::path> mPaths;
        Ui::SystemTab* mTab;
};

#endif
