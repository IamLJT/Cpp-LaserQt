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
    delete gErrorCanvas_1;
    delete gErrorCanvas_2;
    delete gErrorCanvas_3;
    delete gErrorCanvas_4;
    delete gErrorCanvas_5;
    delete gErrorCanvas_6;
    delete gErrorCanvas_7;
    delete gLeftArrowLabel;
    delete gRightArrowLabel;
    delete gStackWin;
    delete gXStart;
    delete gYStart;
    delete gXEnd;
    delete gYEnd;
    delete gOKButton;
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
    gErrorCanvas_1 = new QCustomPlot;
    gErrorCanvas_1->addGraph();
    gErrorCanvas_1->plotLayout()->insertRow(0);
    gErrorCanvas_1->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_1, tr("加工板水平方向1/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_1->xAxis->setVisible(true);
    gErrorCanvas_1->xAxis->setTickLabels(false);
    gErrorCanvas_1->yAxis->setVisible(true);
    gErrorCanvas_1->yAxis->setTickLabels(false);
    gErrorCanvas_2 = new QCustomPlot;
    gErrorCanvas_2->addGraph();
    gErrorCanvas_2->plotLayout()->insertRow(0);
    gErrorCanvas_2->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_2, tr("加工板水平方向1/2处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_2->xAxis->setVisible(true);
    gErrorCanvas_2->xAxis->setTickLabels(false);
    gErrorCanvas_2->yAxis->setVisible(true);
    gErrorCanvas_2->yAxis->setTickLabels(false);
    gErrorCanvas_3 = new QCustomPlot;
    gErrorCanvas_3->addGraph();
    gErrorCanvas_3->plotLayout()->insertRow(0);
    gErrorCanvas_3->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_3, tr("加工板水平方向2/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_3->xAxis->setVisible(true);
    gErrorCanvas_3->xAxis->setTickLabels(false);
    gErrorCanvas_3->yAxis->setVisible(true);
    gErrorCanvas_3->yAxis->setTickLabels(false);
    gErrorCanvas_4 = new QCustomPlot;
    gErrorCanvas_4->addGraph();
    gErrorCanvas_4->plotLayout()->insertRow(0);
    gErrorCanvas_4->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_4, tr("加工板垂直方向1/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_4->xAxis->setVisible(true);
    gErrorCanvas_4->xAxis->setTickLabels(false);
    gErrorCanvas_4->yAxis->setVisible(true);
    gErrorCanvas_4->yAxis->setTickLabels(false);
    gErrorCanvas_5 = new QCustomPlot;
    gErrorCanvas_5->addGraph();
    gErrorCanvas_5->plotLayout()->insertRow(0);
    gErrorCanvas_5->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_5, tr("加工板垂直方向2/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_5->xAxis->setVisible(true);
    gErrorCanvas_5->xAxis->setTickLabels(false);
    gErrorCanvas_5->yAxis->setVisible(true);
    gErrorCanvas_5->yAxis->setTickLabels(false);
    gErrorCanvas_6 = new QCustomPlot;
    gErrorCanvas_6->addGraph();
    gErrorCanvas_6->plotLayout()->insertRow(0);
    gErrorCanvas_6->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_6, tr("加工板任意两点间误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_6->xAxis->setVisible(true);
    gErrorCanvas_6->xAxis->setTickLabels(false);
    gErrorCanvas_6->yAxis->setVisible(true);
    gErrorCanvas_6->yAxis->setTickLabels(false);

    gErrorCanvas_7 = new QtDataVisualization::Q3DScatter;
    gErrorCanvas_7->activeTheme()->setType(QtDataVisualization::Q3DTheme::ThemeEbony);
    gErrorCanvas_7->setShadowQuality(QtDataVisualization::QAbstract3DGraph::ShadowQualitySoftLow);
    gErrorCanvas_7->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetDirectlyBelow);
    gErrorCanvas_7->activeTheme()->setBackgroundColor(QColor(144, 238, 144, 127));
    gErrorCanvas_7->activeTheme()->setBackgroundEnabled(true);
    gErrorCanvas_7->activeTheme()->setGridEnabled(false);
    gErrorCanvas_7->setTitle(tr("全局误差分布"));  // TODO
    gErrorCanvas_7->axisX()->setTitle("X");
    gErrorCanvas_7->axisX()->setTitle("Y");
    gErrorCanvas_7->axisX()->setTitle("Err");

    QWidget * globalErrorCanvas = QWidget::createWindowContainer(gErrorCanvas_7);  // It is important!

    gStackWin = new QStackedWidget;
    gStackWin->addWidget(gErrorCanvas_1);
    gStackWin->addWidget(gErrorCanvas_2);
    gStackWin->addWidget(gErrorCanvas_3);
    gStackWin->addWidget(gErrorCanvas_4);
    gStackWin->addWidget(gErrorCanvas_5);
    gStackWin->addWidget(gErrorCanvas_6);
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
    leftMiddleLayout->setSpacing(20);

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

    QHBoxLayout * leftBottomLayout = new QHBoxLayout;
    leftBottomLayout->setContentsMargins(0, 0, 70, 0);
    leftBottomLayout->addStretch();
    leftBottomLayout->addWidget(gOKButton);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(30);
    leftLayout->addLayout(leftMiddleLayout);
    leftLayout->addSpacing(10);
    leftLayout->addLayout(leftBottomLayout);

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
    #pragma omp parallel for num_threads(4)
    // #pragma omp parallel for
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

    Generate2DMatrix();
    PlotHeatMap();

    kd_free(kd);
}

void FourthWindow::Generate2DMatrix() {
    sort(gEstimators.begin(), gEstimators.end(), CompareX);
    double x_flag = gEstimators.at(0)->x;
    auto cur_it = gEstimators.begin();
    for (auto it = gEstimators.begin(); it != gEstimators.end(); ++it) {
        if ((*it)->x != x_flag) {
            sort(cur_it, it, CompareY);
            x_flag = (*it)->x;
            cur_it = it;
        }
    }
}

void FourthWindow::PlotHeatMap() {
    QtDataVisualization::QScatter3DSeries * series = new QtDataVisualization::QScatter3DSeries;
    series->setMeshSmooth(QtDataVisualization::QAbstract3DSeries::MeshSphere);
    QtDataVisualization::QScatterDataArray data;

    for (size_t i = 0; i < gEstimators.size(); ++i) {
        data << QVector3D(gEstimators.at(i)->x, gEstimators.at(i)->y, gEstimators.at(i)->err);
    }

    series->dataProxy()->addItems(data);

    gErrorCanvas_7->addSeries(series);
}

void FourthWindow::SlotLeftArrowClicked() {
    if (gStackWinIndex == 0) {
        gStackWinIndex = 6;
    } else {
        gStackWinIndex--;
    }
    gStackWin->setCurrentIndex(gStackWinIndex);
    if (gStackWinIndex == 5) {
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
    if (gStackWinIndex == 6) {
        gStackWinIndex = 0;
    } else {
        gStackWinIndex++;
    }
    gStackWin->setCurrentIndex(gStackWinIndex);
    if (gStackWinIndex == 5) {
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
