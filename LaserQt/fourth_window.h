#ifndef FORTHWINDOW_H
#define FORTHWINDOW_H

#include <cmath>
#include <omp.h>
#include <Q3DScatter>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QtConcurrent>
#include <QtPrintSupport>
#include <QVBoxLayout>
#include "auxiliary_function.h"
#include "custom_qlabel.h"
#include "my_messagebox.h"
#include "qcustomplot.h"
#include "kdtree.h"
#include "image_viewer.h"

struct estimator {
    double x;
    double y;
    double err;
    explicit estimator(double _x, double _y, double _err) : x(_x), y(_y), err(_err) {}
    ~estimator() {}
};

class QWidget;

class FourthWindow : public QWidget {
    Q_OBJECT

public:
    explicit FourthWindow(QWidget *parent = 0);
    ~FourthWindow();
    void CopyObjectDataFilePath(const QString &path);
    void clear();

private:
    QCustomPlot * gErrorCanvas_00;
    QCustomPlot * gErrorCanvas_01;
    QCustomPlot * gErrorCanvas_02;
    ClickedQLabel * gLeftArrowLabel;
    ClickedQLabel * gRightArrowLabel;
    QStackedWidget * gStackWin;
    int gStackWinIndex;
    QLineEdit * gGlobalError;
    QLineEdit * gXStart;
    QLineEdit * gYStart;
    QLineEdit * gXEnd;
    QLineEdit * gYEnd;
    QPushButton * gOKButton;
    QLineEdit *  gXDivide;
    QLineEdit *  gYDivide;
    QString gObjectDataFile;
    QtDataVisualization::Q3DScatter * gPointCloudDataGraph;
    vector<struct estimator *> gEstimators;
    vector<struct estimator *> gEstimatorsAccordingToX;  // TODO
    vector<struct estimator *> gEstimatorsAccordingToY;  // TODO
    QStringList gErrorImages;

    void CreateMainWindow();
    void SetWidgets();
    void InitWindow();
    void InitErrorAnalysis();
    void EstimateError();
    void Generate2DMatrixAccordingToX();
    void Generate2DMatrixAccordingToY();
    void PlotX(const qint32 &split);
    void PlotY(const qint32 &split);
    void PlotHeatMap();
    void ClearGraph();

private slots:
    void SlotLeftArrowClicked();
    void SlotRightArrowClicked();
    void SlotOK();
    void SlotEstimate();
};

#endif // FORTHWINDOW_H
