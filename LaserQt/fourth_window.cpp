#include "fourth_window.h"

FourthWindow::FourthWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

FourthWindow::~FourthWindow() {

}

void FourthWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}

void FourthWindow::SetWidgets() {}
