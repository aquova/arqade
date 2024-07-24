#include "KeybindWindow.hpp"
#include "ui_KeybindWindow.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "Database.hpp"

KeybindWindow::KeybindWindow(const int aTabIndex, QWidget* aParent) : QWidget(aParent), mTabIndex(aTabIndex), mUi(new Ui::KeybindWindow) {
    mUi->setupUi(this);
    mData.mType = DbGetControllerType(mTabIndex);
    PopulateControllers();

    connect(mUi->controllerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KeybindWindow::HandleControllerChanged);
    connect(mUi->saveButton, &QPushButton::pressed, this, &KeybindWindow::HandleSaveButtonPressed);
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
            const auto index = mPressedButton->property("index").toInt();
            const auto key = static_cast<Qt::Key>(key_event->key());
            // Prevent duplicate button bindings
            for (auto& button : mData.mButtons) {
                if (button == key) {
                    button = Qt::Key_unknown;
                    break;
                }
            }
            mData.mButtons[index] = key;
            mPressedButton = nullptr;
            LoadControllerLayout();
            return true;
        }
    }
    return QObject::eventFilter(aObject, aEvent);
}

void KeybindWindow::HandleControllerChanged(int aIdx) {
    mData.mType = static_cast<ControllerType>(aIdx);
    mData.mButtons = DbGetKeybind(mTabIndex, mData.mType);
    LoadControllerLayout();
}

void KeybindWindow::HandleKeyListenPressed() {
    if (mPressedButton == nullptr) {
        mPressedButton = qobject_cast<QPushButton*>(sender());
        mPressedButton->setText("...");
        mPressedButton->installEventFilter(this);
    }
}

void KeybindWindow::HandleSaveButtonPressed() {
    DbUpdateKeybind(mTabIndex, mData);
    close();
}

void KeybindWindow::LoadControllerLayout() {
    const auto controller_info = ControllerTable[static_cast<int>(mData.mType)];

    QWidget* controller_widget = new QWidget;
    QVBoxLayout* v_box = new QVBoxLayout(controller_widget);
    for (size_t i = 0; i < mData.mButtons.size(); i++) {
        QHBoxLayout* h_box = new QHBoxLayout;

        auto label = new QLabel(controller_info.mButtonNames.at(i).c_str());

        auto key_button = new QPushButton;
        const auto button_key = mData.mButtons[i];
        if (button_key != Qt::Key_unknown) {
            const auto text = QKeySequence(button_key).toString();
            key_button->setText(text);
        }
        key_button->installEventFilter(this);
        key_button->setProperty("index", static_cast<int>(i));
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
    mUi->controllerComboBox->setCurrentIndex(static_cast<int>(mData.mType));
    mData.mButtons = DbGetKeybind(mTabIndex, mData.mType);
    LoadControllerLayout();
}
