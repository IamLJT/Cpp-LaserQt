#ifndef STACK_WINDOW_H
#define STACK_WINDOW_H

#include <QHBoxLayout>
#include <QStackedWidget>
#include "main_window.h"
#include "second_window.h"
#include "third_window.h"
#include "fourth_window.h"
#include "auxiliary_function.h"

class StackWindow : public QWidget {
    Q_OBJECT

public:
    explicit StackWindow(QWidget * parent = 0);
    ~StackWindow();

private:
    MainWindow * gMainWin;
    SecondWindow * gSecondWin;
    ThirdWindow * gThirdWin;
    FourthWindow * gFourthWin;
    QStackedWidget * gStackWin;
    QPushButton * gPrevButton;
    QPushButton * gNextButton;
    int gStackWinIndex;

    void CreateMainWindow();
    void SetWidgets();

private slots:
    void SlotPrev();
    void SlotNext();
    void SlotQuit();
};

#endif // STACK_WINDOW_H
