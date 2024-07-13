#include "KeybindWindow.hpp"
#include "ui_KeybindWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

KeybindWindow::KeybindWindow(ControllerType aType, QWidget* aParent) : QWidget(aParent), mControllerType(aType), mUi(new Ui::KeybindWindow) {
    mUi->setupUi(this);
    PopulateControllers();

    connect(mUi->controllerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KeybindWindow::HandleControllerChanged);
}

KeybindWindow::~KeybindWindow() {
    delete mUi;
}

void KeybindWindow::closeEvent(QCloseEvent* aEvent) {
    emit WindowClosing();
    QWidget::closeEvent(aEvent);
}

bool KeybindWindow::eventFilter(QObject* aObject, QEvent* aEvent) {
    if (aEvent->type() == QEvent::KeyPress) {
        const auto key_event = static_cast<QKeyEvent*>(aEvent);
        const auto txt = QKeySequence(key_event->key()).toString();
        if (!txt.isEmpty()) {
            mPressedButton->setText(txt);
            mPressedButton->removeEventFilter(this);
            mPressedButton = nullptr;
            return true;
        }
    }
    return QObject::eventFilter(aObject, aEvent);
}

void KeybindWindow::HandleControllerChanged(int aIdx) {
    LoadControllerLayout(aIdx);
}

void KeybindWindow::HandleKeyListenPressed() {
    if (mPressedButton == nullptr) {
        mPressedButton = qobject_cast<QPushButton*>(sender());
        mPressedButton->setText("...");
        mPressedButton->installEventFilter(this);
    }
}

void KeybindWindow::LoadControllerLayout(const int aIdx) {
    QWidget* controller_widget = new QWidget;
    QVBoxLayout* v_box = new QVBoxLayout(controller_widget);
    const auto controller_info = ControllerTable[aIdx];
    for (size_t i = 0; i < controller_info.mButtons.size(); i++) {
        QHBoxLayout* h_box = new QHBoxLayout;

        auto label = new QLabel(controller_info.mButtonNames.at(i).c_str());
        auto key_button = new QPushButton;
        key_button->installEventFilter(this);
        connect(key_button, &QPushButton::clicked, this, &KeybindWindow::HandleKeyListenPressed);

        h_box->addWidget(label);
        h_box->addWidget(key_button);
        v_box->addLayout(h_box);
    }
    v_box->addStretch(); // Required so items are positioned to top and not center
    delete mUi->stackedWidget->currentWidget();
    mUi->stackedWidget->addWidget(controller_widget);
    mUi->stackedWidget->setCurrentWidget(controller_widget);
}

void KeybindWindow::PopulateControllers() {
    for (const auto& type : ControllerTable) {
        mUi->controllerComboBox->addItem(type.mName.c_str());
    }
    LoadControllerLayout(0);
}
