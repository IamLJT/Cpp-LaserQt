#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H

#include "auxiliary_function.h"
#include <QMainWindow>

class QWidget;

class ThirdWindow : public QWidget {
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = 0);
    ~ThirdWindow();

private:

};

#endif // THIRDWINDOW_H
