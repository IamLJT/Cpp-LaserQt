#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cmath>
#include <QCoreApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include "auxiliary_function.h"
#include "my_messagebox.h"
#include "qcustomplot.h"

/* Linux */
/*
#ifdef _WIN32
  #include <windows.h>
#endif
#include "libxl.h"
*/

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();
    void clear();

private:
    QLineEdit * gInputFile;
    QTableWidget * gDataTable;
    QPushButton * gEditButton;
    QPushButton * gUpdateButton;
    bool gEditFlag;
    QCustomPlot * gCustomPlot;

    void CreateMainWindow();
    void SetWidgets();
    void Plot();

private slots:
    void SlotOpenFile();
    void SlotEditTable();
    void SlotUpdateTable();
};

#endif // MAINWINDOW_H
