#include "third_window.h"

ThirdWindow::ThirdWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

ThirdWindow::~ThirdWindow() {

}

void ThirdWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}


void ThirdWindow::SetWidgets() {}
