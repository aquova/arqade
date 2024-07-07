#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>
#include <QPushButton>

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
        void HandleRunButtonPressed();

    private:
        void AddTab();
        void CreateConfig();
        void PopulateTabs();

        Ui::ArqadeWindow *mUi;
};
#endif
