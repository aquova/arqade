#ifndef WINDOW_H
#define WINDOW_H

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
        void HandleCoreButtonPressed();
        void HandleGameButtonPressed();
        void HandleRunButtonPressed();

    private:
        Ui::ArqadeWindow *mUi;
        bool mCoreSelected{false};
        bool mGameSelected{false};
};
#endif // WINDOW_H
