#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H

#include <Q3DScatter>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QTextEdit>
#include <QtXml>
#include <QVBoxLayout>
#include "auxiliary_function.h"
#include "my_messagebox.h"
#include "qcustomplot.h"

class QWidget;

class ThirdWindow : public QWidget {
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = 0);
    ~ThirdWindow();
    QString CopyObjectDataFilePath();
    void clear();

private:
    QLineEdit * gObjectDataFile;
    QLineEdit * gScanningDataFile;
    QTextEdit * gLogger;
    QProgressBar * gExecuteProgressBar;
    QComboBox * gCameraPresentList;
    QPushButton * gPointCloudDataScanningButton;
    QPushButton * gPointCloudDataDenoisingButton;
    QPushButton * gPointCloudDataFittingButton;
    QtDataVisualization::Q3DScatter * gPointCloudDataGraph;
    QtDataVisualization::QScatter3DSeries * series1;
    QtDataVisualization::QScatter3DSeries * series2;

    void CreateMainWindow();
    void SetWidgets();
    void ClearGraph();

private slots:
    void SlotOpenObjectDataFile();
    void SlotOpenScanningDataFile();
    void SlotScanPointCloudData();
    void SlotDenoisePointCloudData();
    void SlotFitPointCloudData();
    void SlotAdjustCameraPresent();
};

#endif // THIRDWINDOW_H
