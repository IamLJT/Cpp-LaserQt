#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include "auxiliary_function.h"
#include <QMainWindow>

class QWidget;

class SecondWindow : public QWidget {
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = 0);
    ~SecondWindow();

private:

};

#endif // SECONDWINDOW_H
