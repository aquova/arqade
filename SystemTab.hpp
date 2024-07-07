#ifndef SYSTEMTAB_HPP
#define SYSTEMTAB_HPP

#include <filesystem>
#include <vector>

#include <QWidget>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE namespace Ui {
class SystemTab;
}
QT_END_NAMESPACE

class SystemTab :  public QWidget {
    Q_OBJECT

    public:
        SystemTab(QWidget *aParent = nullptr);
        ~SystemTab();
        std::string GetSelectedCore();
        std::string GetSelectedGame();

    private:
        void PopulateCores();
        void PopulateList();
        void UpdateGames();

        std::vector<std::string> mGames;
        std::vector<std::filesystem::path> mPaths;
        Ui::SystemTab* mTab;
};

#endif
