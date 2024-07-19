#ifndef KEYBINDWINDOW_HPP
#define KEYBINDWINDOW_HPP

#include <QCloseEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QWidget>

#include "ControllerData.hpp"

QT_BEGIN_NAMESPACE namespace Ui {
class KeybindWindow;
}
QT_END_NAMESPACE

class KeybindWindow : public QWidget {
    Q_OBJECT

    public:
        KeybindWindow(const int aTabIndex, QWidget* aParent = nullptr);
        ~KeybindWindow();

    signals:
        void WindowClosing();

    private slots:
        void closeEvent(QCloseEvent* aEvent) override;
        bool eventFilter(QObject* aObject, QEvent* aEvent) override;
        void HandleControllerChanged(int aIdx);
        void HandleKeyListenPressed();
        void HandleSaveButtonPressed();

    private:
        void LoadControllerLayout();
        void PopulateControllers();

        ControllerData mData;
        int mTabIndex;
        QPushButton* mPressedButton{nullptr};
        Ui::KeybindWindow* mUi;
};

#endif
