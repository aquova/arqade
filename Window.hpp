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
        void HandleGameButtonPressed();
        void HandleRunButtonPressed();

    private:
        void CreateConfig();
        void PopulateCores();

        bool mGameSelected{false};
        Ui::ArqadeWindow *mUi;
};
#endif
