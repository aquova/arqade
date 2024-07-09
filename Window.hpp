#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QString>

QT_BEGIN_NAMESPACE namespace Ui {
class ArqadeWindow;
}
QT_END_NAMESPACE

class ArqadeWindow : public QMainWindow {
    Q_OBJECT

    public:
        ArqadeWindow(QWidget *parent = nullptr);
        ~ArqadeWindow();

    private slots:
        void HandleAddTabPressed();
        void HandleDeleteTabPressed();
        void HandleRunButtonPressed();
        void HandleUpdateTitle(QString aTitle);

    private:
        void AddTab(const std::string aTitle, const int aIdx);
        void CreateConfig();
        void CreateDefaultTabs();
        void PopulateTabs();

        std::vector<std::string> mCores;
        Ui::ArqadeWindow *mUi;
};
#endif
