#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <cmath>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QUdpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QtXml>
#include "auxiliary_function.h"
#include "my_messagebox.h"
#include "qcustomplot.h"

#ifdef _WIN32
  #include <QtXlsx>
#elif __linux__
  #include "libxl.h"
#endif

#include "QJson"

class QWidget;

class SecondWindow : public QWidget {
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = 0);
    ~SecondWindow();
    void clear();

private:
    QLineEdit * gInputFile;
    QLineEdit * gPathIndex;
    QLineEdit * gStartCoord;
    QLineEdit * gEndCoord;
    QLineEdit * gReduction;
    QLineEdit * gThermalParameter;
    QLineEdit * gProcessingTime;
    QPushButton * gStartProcessingButton;
    QPushButton * gStopProcessingButton;
    QPushButton * gContinueProcessingButton;
    QCustomPlot * gCustomPlot;
    QTextEdit * gLogger;
    QQueue<QVector<double>> * gTaskQueue;
    QVector<QVector<double>> * gTaskList;
    QTimer * gTimer;
    int32_t gCounter;
    bool gIsStop;
    QString gProcessMachineIP;
    int16_t gProcessMachinePort;
    QHostAddress gHostAddress;
    QUdpSocket * gUDPSocket;
    bool recvDone;

    void CreateMainWindow();
    void SetWidgets();
    void InitTaskQueue();
    void Sleep(const size_t &msec);
    void InitSocket();
    void ClearGraph();

private slots:
    void SlotOpenFile();
    void SlotStartProcessing();
    void SlotStopProcessing();
    void SlotContinueProcessing();
    void SlotUpdateTime();
    void SlotReadPendingDatagrams();
};

#endif // SECONDWINDOW_H
