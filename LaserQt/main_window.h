#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cmath>
#include <QCoreApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVector>
#include "auxiliary_function.h"
#include "my_messagebox.h"
#include "qcustomplot.h"

#ifdef _WIN32
  #include <windows.h>
#endif

#include "libxl.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

private:
    QLineEdit * gInputFile;
    QTableWidget * gDataTable;
    QPushButton * gEditButton;
    bool gEditFlag;
    QCustomPlot * gCustomPlot;

    void CreateMainWindow();
    void SetWidgets();
    void Plot();

private slots:
    void SlotOpenFile();
    void SlotEditTable();
    void SlotUpdateTable();
    void SlotNext();
    void SlotQuit();
};

#endif // MAINWINDOW_H
