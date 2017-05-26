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
#include "auxiliary_function.h"
#include "my_messagebox.h"
#include "qcustomplot.h"
#ifdef _WIN32
  #include <windows.h>
#endif
#include "libxl.h"
#include "yaml.h"
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
    QTimer * gTimer;
    int32_t gCounter;
    bool gIsStop;
    QString gProcessMachineIP;
    int16_t gProcessMachinePort;
    QHostAddress hostAddress;
    QUdpSocket * gUDPSocket;
    bool recvDone;

    void CreateMainWindow();
    void SetWidgets();
    void InitTaskQueue();
    void Sleep(size_t msec);
    void InitSocket();

private slots:
    void SlotOpenFile();
    void SlotStartProcessing();
    void SlotStopProcessing();
    void SlotContinueProcessing();
    void SlotUpdateTime();
    void SlotReadPendingDatagrams();
};

#endif // SECONDWINDOW_H
