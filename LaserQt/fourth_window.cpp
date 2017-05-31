#include "fourth_window.h"
#include <algorithm>

bool CompareX(struct estimator * a, struct estimator * b) {
    return a->x < b->x;
}

bool CompareY(struct estimator * a, struct estimator * b) {
    return a->y < b->y;
}

FourthWindow::FourthWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

FourthWindow::~FourthWindow() {
    delete gErrorCanvas_00;
    delete gErrorCanvas_01;
    delete gErrorCanvas_02;
    delete gErrorCanvas_03;
    delete gLeftArrowLabel;
    delete gRightArrowLabel;
    delete gStackWin;
    delete gXStart;
    delete gYStart;
    delete gXEnd;
    delete gYEnd;
    delete gOKButton;
    delete gXDivide;
    delete gYDivide;
    delete gPointCloudDataGraph;

    for (size_t i = 0; i < gEstimators.size(); ++i) {
        delete gEstimators.at(i);
    }
}

void FourthWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}

void FourthWindow::SetWidgets() {
    /* left-top layout */
    gErrorCanvas_00 = new QCustomPlot;
    gErrorCanvas_00->addGraph();
    gErrorCanvas_00->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    gErrorCanvas_00->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    gErrorCanvas_00->plotLayout()->insertRow(0);
    gErrorCanvas_00->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_00, tr("水平方向局部误差图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_00->xAxis->setVisible(true);
    gErrorCanvas_00->xAxis->setTickLabels(false);
    gErrorCanvas_00->yAxis->setVisible(true);
    gErrorCanvas_00->yAxis->setTickLabels(false);
    gErrorCanvas_01 = new QCustomPlot;
    gErrorCanvas_01->addGraph();
    gErrorCanvas_01->plotLayout()->insertRow(0);
    gErrorCanvas_01->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_01, tr("垂直方向局部误差图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_01->xAxis->setVisible(true);
    gErrorCanvas_01->xAxis->setTickLabels(false);
    gErrorCanvas_01->yAxis->setVisible(true);
    gErrorCanvas_01->yAxis->setTickLabels(false);
    gErrorCanvas_02 = new QCustomPlot;
    gErrorCanvas_02->addGraph();
    gErrorCanvas_02->plotLayout()->insertRow(0);
    gErrorCanvas_02->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_02, tr("任意两点间误差图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_02->xAxis->setVisible(true);
    gErrorCanvas_02->xAxis->setTickLabels(false);
    gErrorCanvas_02->yAxis->setVisible(true);
    gErrorCanvas_02->yAxis->setTickLabels(false);

    gErrorCanvas_03 = new QtDataVisualization::Q3DScatter;
    gErrorCanvas_03->activeTheme()->setType(QtDataVisualization::Q3DTheme::ThemeQt);
    gErrorCanvas_03->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftLow);
    gErrorCanvas_03->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetDirectlyAbove);
    gErrorCanvas_03->activeTheme()->setGridEnabled(false);
    gErrorCanvas_03->setTitle(tr("全局误差分布"));  // TODO

    QWidget * globalErrorCanvas = QWidget::createWindowContainer(gErrorCanvas_03);  // It is important!

    gStackWin = new QStackedWidget;
    gStackWin->addWidget(gErrorCanvas_00);
    gStackWin->addWidget(gErrorCanvas_01);
    gStackWin->addWidget(gErrorCanvas_02);
    gStackWin->addWidget(globalErrorCanvas);
    gStackWinIndex = 0;
    gStackWin->setCurrentIndex(gStackWinIndex);

    gLeftArrowLabel = new ClickedQLabel();
    gLeftArrowLabel->setPixmap(QPixmap::fromImage(QImage(":/img/ui/arrow_left.png")));
    gLeftArrowLabel->setAlignment(Qt::AlignCenter);
    connect(gLeftArrowLabel, SIGNAL(clicked()), this, SLOT(SlotLeftArrowClicked()));
    gRightArrowLabel = new ClickedQLabel();
    gRightArrowLabel->setPixmap(QPixmap::fromImage(QImage(":/img/ui/arrow_right.png")));
    gRightArrowLabel->setAlignment(Qt::AlignCenter);
    connect(gRightArrowLabel, SIGNAL(clicked()), this, SLOT(SlotRightArrowClicked()));

    QHBoxLayout * leftTopLayout = new QHBoxLayout;
    leftTopLayout->addWidget(gLeftArrowLabel);
    leftTopLayout->addWidget(gStackWin);
    leftTopLayout->addWidget(gRightArrowLabel);

    /* left-middle layout */
    QGridLayout * leftMiddleLayout = new QGridLayout;
    leftMiddleLayout->setContentsMargins(70, 0, 70, 0);
    leftMiddleLayout->setSpacing(10);

    leftMiddleLayout->addWidget(new QLabel(tr("起点Ｘ坐标")), 0, 0);
    gXStart = new QLineEdit;
    gXStart->setEnabled(false);
    leftMiddleLayout->addWidget(gXStart, 0, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("起点Y坐标")), 0, 2);
    gYStart = new QLineEdit;
    gYStart->setEnabled(false);
    leftMiddleLayout->addWidget(gYStart, 0, 3);
    leftMiddleLayout->addWidget(new QLabel(tr("终点Ｘ坐标")), 1, 0);
    gXEnd = new QLineEdit;
    gXEnd->setEnabled(false);
    leftMiddleLayout->addWidget(gXEnd, 1, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("终点Y坐标")), 1, 2);
    gYEnd = new QLineEdit;
    gYEnd->setEnabled(false);
    leftMiddleLayout->addWidget(gYEnd, 1, 3);

    /* left-bottom layout */
    gOKButton = new QPushButton(tr("确定"));
    gOKButton->setEnabled(false);
    connect(gOKButton, SIGNAL(clicked()), this, SLOT(SlotOK()));
    leftMiddleLayout->addWidget(gOKButton, 1, 4);

    leftMiddleLayout->addWidget(new QLabel(tr("X方向均分")), 2, 0);
    gXDivide = new QLineEdit;
    leftMiddleLayout->addWidget(gXDivide, 2, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("Y方向均分")), 2, 2);
    gYDivide = new QLineEdit;
    leftMiddleLayout->addWidget(gYDivide, 2, 3);

    QPushButton * estimateButton = new QPushButton(tr("评估"));
    connect(estimateButton, SIGNAL(clicked()), this, SLOT(SlotEstimate()));
    leftMiddleLayout->addWidget(estimateButton, 2, 4);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(30);
    leftLayout->addLayout(leftMiddleLayout);

    /* right layout */
    gPointCloudDataGraph = new QtDataVisualization::Q3DScatter;
    gPointCloudDataGraph->activeTheme()->setType(QtDataVisualization::Q3DTheme::ThemeEbony);
    gPointCloudDataGraph->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftLow);
    gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetIsometricRight);
    gPointCloudDataGraph->activeTheme()->setBackgroundColor(QColor(144, 238, 144, 127));
    gPointCloudDataGraph->activeTheme()->setBackgroundEnabled(true);
    gPointCloudDataGraph->activeTheme()->setGridEnabled(false);
    gPointCloudDataGraph->setTitle(tr("点云渲染图"));  // TODO
    gPointCloudDataGraph->axisX()->setTitle("X");
    gPointCloudDataGraph->axisX()->setTitle("Y");
    gPointCloudDataGraph->axisX()->setTitle("Z");

    QWidget * container = QWidget::createWindowContainer(gPointCloudDataGraph);  // It is important!

    QVBoxLayout * rightLayout = new QVBoxLayout;
    rightLayout->addWidget(container);

    /* main layout */
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(leftLayout, 1);
    layout->setStretchFactor(rightLayout, 1);

    setLayout(layout);
}

void FourthWindow::CopyObjectDataFilePath(QString path) {
    gObjectDataFile = path;
}

void FourthWindow::InitWindow() {
    QtDataVisualization::QScatter3DSeries * series1 = new QtDataVisualization::QScatter3DSeries;
    series1->setMeshSmooth(QtDataVisualization::QAbstract3DSeries::MeshPoint);
    QtDataVisualization::QScatterDataArray data1;
    QFile f1(gObjectDataFile);
    if (f1.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream txtInput(&f1);
        QString line;
        QStringList items;
        while (!txtInput.atEnd()) {
            line = txtInput.readLine();
            items = line.split(",");
            data1 << QVector3D(items.at(0).toDouble(), items.at(1).toDouble(), items.at(2).toDouble());
        }
        f1.close();
    }
    series1->dataProxy()->addItems(data1);

    QtDataVisualization::QScatter3DSeries * series2 = new QtDataVisualization::QScatter3DSeries;
    series2->setMeshSmooth(QtDataVisualization::QAbstract3DSeries::MeshSphere);
    QtDataVisualization::QScatterDataArray data2;
    QFile f2(":/cache/cache/输出数据.txt");
    if (f2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream txtInput(&f2);
        QString line;
        QStringList items;
        while (!txtInput.atEnd()) {
            line = txtInput.readLine();
            items = line.split(",");
            data2 << QVector3D(items.at(0).toDouble(), items.at(1).toDouble(), items.at(2).toDouble());
        }
        f2.close();
    }
    series2->dataProxy()->addItems(data2);

    gPointCloudDataGraph->addSeries(series1);
    gPointCloudDataGraph->addSeries(series2);
    InitErrorAnalysis();
}

void FourthWindow::InitErrorAnalysis() {
    MyMessageBox msgBox;
    msgBox.setText(tr("是否进行曲面误差评估?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Yes) {
        EstimateError();
    }
}

void FourthWindow::EstimateError() {
    struct kdtree * kd = kd_create(3);

    QFile f1(":/cache/cache/输出数据.txt");
    if (f1.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream txtInput(&f1);
        QString line;
        QStringList items;
        while (!txtInput.atEnd()) {
            line = txtInput.readLine();
            items = line.split(",");
            kd_insert3(kd, items.at(0).toDouble(), items.at(1).toDouble(), items.at(2).toDouble(), 0);
        }
        f1.close();
    }

    QVector<QVector3D> * points = new QVector<QVector3D>;
    QFile f2(gObjectDataFile);
    if (f2.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream txtInput(&f2);
        QString line;
        QStringList items;
        while (!txtInput.atEnd()) {
            line = txtInput.readLine();
            items = line.split(",");
            points->push_back(QVector3D(items.at(0).toDouble(), items.at(1).toDouble(), items.at(2).toDouble()));
        }
        f2.close();
    }

    gEstimators.resize(points->size());
    // #pragma omp parallel for num_threads(4)
    #pragma omp parallel for
    for (int i = 0; i < points->size(); ++i) {
        struct kdres * result_set = kd_nearest3(kd, points->at(i).x(), points->at(i).y(), points->at(i).z());
        double pos[3];
        kd_res_item(result_set, pos);
        double err = sqrt((points->at(i).x() - pos[0]) * (points->at(i).x() - pos[0]) +
                (points->at(i).y() - pos[1]) * (points->at(i).y() - pos[1]) +
                (points->at(i).z() - pos[2]) * (points->at(i).z() - pos[2]));
        struct estimator * est = new struct estimator(points->at(i).x(), points->at(i).y(), err);
        gEstimators[i] = est;
        kd_res_free(result_set);
    }
    delete points;
    kd_free(kd);

    Generate2DMatrixAccordingToX();
    PlotHeatMap();
}

void FourthWindow::Generate2DMatrixAccordingToX() {
    sort(gEstimators.begin(), gEstimators.end(), CompareX);

    QVector<QVector2D> points;

    double x_flag = gEstimators.at(0)->x;
    double err_max = gEstimators.at(0)->err;
    auto cur_it = gEstimators.begin();
    int i = 1;
    for (auto it = gEstimators.begin(); it != gEstimators.end(); ++it) {
        if ((*it)->x != x_flag) {
            points.push_back(QVector2D(i, err_max));
            i++;
            err_max = (*it)->err;
            x_flag = (*it)->x;

            sort(cur_it, it, CompareY);

            cur_it = it;
        } else {
            if ((*it)->err > err_max) {
                err_max = (*it)->err;
            }
        }
    }

    for (auto it = points.begin(); it != points.end(); ++it) {
        gErrorCanvas_00->graph(0)->addData((*it).x(), (*it).y());
    }
    gErrorCanvas_00->xAxis->setTickLabels(true);
    gErrorCanvas_00->xAxis->setLabel(tr("水平方向"));
    gErrorCanvas_00->yAxis->setTickLabels(true);
    gErrorCanvas_00->yAxis->setLabel(tr("误差(m)"));
    gErrorCanvas_00->xAxis->setRange(1, i);
    gErrorCanvas_00->yAxis->setRange(0, 0.1);
    gErrorCanvas_00->replot();
}

void FourthWindow::Generate2DMatrixAccordingToY() {}

void FourthWindow::PlotHeatMap() {
    QtDataVisualization::QScatter3DSeries * series = new QtDataVisualization::QScatter3DSeries;
    series->setMeshSmooth(QtDataVisualization::QAbstract3DSeries::MeshSphere);
    QtDataVisualization::QScatterDataArray data;

    for (size_t i = 0; i < gEstimators.size(); ++i) {
        data << QVector3D(gEstimators.at(i)->x, gEstimators.at(i)->y, gEstimators.at(i)->err);
    }

    series->dataProxy()->addItems(data);

    gErrorCanvas_03->addSeries(series);
}

void FourthWindow::SlotLeftArrowClicked() {
    if (gStackWinIndex == 0) {
        gStackWinIndex = 3;
    } else {
        gStackWinIndex--;
    }
    gStackWin->setCurrentIndex(gStackWinIndex);
    if (gStackWinIndex == 2) {
        gXStart->setEnabled(true);
        gYStart->setEnabled(true);
        gXEnd->setEnabled(true);
        gYEnd->setEnabled(true);
        gOKButton->setEnabled(true);
    } else {
        gXStart->clear();
        gXStart->setEnabled(false);
        gYStart->clear();
        gYStart->setEnabled(false);
        gXEnd->clear();
        gXEnd->setEnabled(false);
        gYEnd->clear();
        gYEnd->setEnabled(false);
        gOKButton->setEnabled(false);
    }
}

void FourthWindow::SlotRightArrowClicked() {
    if (gStackWinIndex == 3) {
        gStackWinIndex = 0;
    } else {
        gStackWinIndex++;
    }
    gStackWin->setCurrentIndex(gStackWinIndex);
    if (gStackWinIndex == 2) {
        gXStart->setEnabled(true);
        gYStart->setEnabled(true);
        gXEnd->setEnabled(true);
        gYEnd->setEnabled(true);
        gOKButton->setEnabled(true);
    } else {
        gXStart->clear();
        gXStart->setEnabled(false);
        gYStart->clear();
        gYStart->setEnabled(false);
        gXEnd->clear();
        gXEnd->setEnabled(false);
        gYEnd->clear();
        gYEnd->setEnabled(false);
        gOKButton->setEnabled(false);
    }
}

void FourthWindow::SlotOK() {}

void FourthWindow::SlotEstimate() {}
