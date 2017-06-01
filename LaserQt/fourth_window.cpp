#include "fourth_window.h"
#include <algorithm>
#include <QDebug>

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
    delete gLeftArrowLabel;
    delete gRightArrowLabel;
    delete gStackWin;
    delete gGlobalError;
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

void FourthWindow::clear() {
    ClearGraph();
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
    gErrorCanvas_00->graph(0)->setPen(QPen(QColor(250, 128, 114)));
    gErrorCanvas_00->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    gErrorCanvas_00->plotLayout()->insertRow(0);
    gErrorCanvas_00->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_00, tr("水平方向局部误差图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_00->xAxis->setVisible(true);
    gErrorCanvas_00->xAxis->setTickLabels(false);
    gErrorCanvas_00->yAxis->setVisible(true);
    gErrorCanvas_00->yAxis->setTickLabels(false);
    gErrorCanvas_01 = new QCustomPlot;
    gErrorCanvas_01->addGraph();
    gErrorCanvas_01->graph(0)->setPen(QPen(QColor(250, 128, 114)));
    gErrorCanvas_01->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
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

    gStackWin = new QStackedWidget;
    gStackWin->addWidget(gErrorCanvas_00);
    gStackWin->addWidget(gErrorCanvas_01);
    gStackWin->addWidget(gErrorCanvas_02);
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

    leftMiddleLayout->addWidget(new QLabel(tr("全局误差/m")), 0, 0);
    gGlobalError = new QLineEdit;
    gGlobalError->setEnabled(false);
    leftMiddleLayout->addWidget(gGlobalError, 0, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("起点Ｘ坐标")), 1, 0);
    gXStart = new QLineEdit;
    gXStart->setEnabled(false);
    leftMiddleLayout->addWidget(gXStart, 1, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("起点Y坐标")), 1, 2);
    gYStart = new QLineEdit;
    gYStart->setEnabled(false);
    leftMiddleLayout->addWidget(gYStart, 1, 3);
    leftMiddleLayout->addWidget(new QLabel(tr("终点Ｘ坐标")), 2, 0);
    gXEnd = new QLineEdit;
    gXEnd->setEnabled(false);
    leftMiddleLayout->addWidget(gXEnd, 2, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("终点Y坐标")), 2, 2);
    gYEnd = new QLineEdit;
    gYEnd->setEnabled(false);
    leftMiddleLayout->addWidget(gYEnd, 2, 3);

    /* left-bottom layout */
    gOKButton = new QPushButton(tr("确定"));
    gOKButton->setEnabled(false);
    connect(gOKButton, SIGNAL(clicked()), this, SLOT(SlotOK()));
    leftMiddleLayout->addWidget(gOKButton, 2, 4);

    leftMiddleLayout->addWidget(new QLabel(tr("X方向均分")), 3, 0);
    gXDivide = new QLineEdit;
    leftMiddleLayout->addWidget(gXDivide, 3, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("Y方向均分")), 3, 2);
    gYDivide = new QLineEdit;
    leftMiddleLayout->addWidget(gYDivide, 3, 3);

    QPushButton * estimateButton = new QPushButton(tr("评估"));
    connect(estimateButton, SIGNAL(clicked()), this, SLOT(SlotEstimate()));
    leftMiddleLayout->addWidget(estimateButton, 3, 4);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(10);
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

void FourthWindow::ClearGraph() {}

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

    double err_average = 0.0;
    #pragma omp parallel for reduction(+:err_average)
    for (size_t i = 0; i < gEstimators.size(); ++i) {
        err_average += gEstimators.at(i)->err * gEstimators.at(i)->err;
    }
    err_average /= gEstimators.size();
    err_average = sqrt(err_average);  // 全局误差
    gGlobalError->setText(QString::number(err_average));

    Generate2DMatrixAccordingToX();
    Generate2DMatrixAccordingToY();
    PlotHeatMap();
}

void FourthWindow::Generate2DMatrixAccordingToX() {
    gEstimatorsAccordingToX = gEstimators;

    sort(gEstimatorsAccordingToX.begin(), gEstimatorsAccordingToX.end(), CompareX);

    QVector<QVector2D> points;

    double x_flag = gEstimatorsAccordingToX.at(0)->x;
    double err_max = gEstimatorsAccordingToX.at(0)->err;
    auto cur_it = gEstimatorsAccordingToX.begin();
    int i = 1;
    for (auto it = gEstimatorsAccordingToX.begin(); it != gEstimatorsAccordingToX.end(); ++it) {
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

    // gErrorCanvas_00->saveJpg(tr("D:/我的文档/Qt/水平方向局部误差图.jpg"), 488, 485, 1, -1);  // TODO
}

void FourthWindow::Generate2DMatrixAccordingToY() {
    gEstimatorsAccordingToY = gEstimators;

    sort(gEstimatorsAccordingToY.begin(), gEstimatorsAccordingToY.end(), CompareY);

    QVector<QVector2D> points;

    double y_flag = gEstimatorsAccordingToY.at(0)->y;
    double err_max = gEstimatorsAccordingToY.at(0)->err;
    auto cur_it = gEstimatorsAccordingToY.begin();
    int i = 1;
    for (auto it = gEstimatorsAccordingToY.begin(); it != gEstimatorsAccordingToY.end(); ++it) {
        if ((*it)->y != y_flag) {
            points.push_back(QVector2D(i, err_max));
            i++;
            err_max = (*it)->err;
            y_flag = (*it)->y;

            sort(cur_it, it, CompareX);

            cur_it = it;
        } else {
            if ((*it)->err > err_max) {
                err_max = (*it)->err;
            }
        }
    }

    for (auto it = points.begin(); it != points.end(); ++it) {
        gErrorCanvas_01->graph(0)->addData((*it).x(), (*it).y());
    }
    gErrorCanvas_01->xAxis->setTickLabels(true);
    gErrorCanvas_01->xAxis->setLabel(tr("垂直方向"));
    gErrorCanvas_01->yAxis->setTickLabels(true);
    gErrorCanvas_01->yAxis->setLabel(tr("误差(m)"));
    gErrorCanvas_01->xAxis->setRange(1, i);
    gErrorCanvas_01->yAxis->setRange(0, 0.1);
    gErrorCanvas_01->replot();

    // gErrorCanvas_01->saveJpg(tr("D:/我的文档/Qt/垂直方向局部误差图.jpg"), 488, 485, 1, -1);  // TODO
}

void FourthWindow::PlotX(qint32 split) {
    vector<double> x;
    double cur_x = gEstimatorsAccordingToX.at(0)->x;
    x.push_back(cur_x);
    for (size_t i = 0; i < gEstimatorsAccordingToX.size(); ++i) {
        if (cur_x != gEstimatorsAccordingToX.at(i)->x) {
            x.push_back(cur_x);
            cur_x = gEstimatorsAccordingToX.at(i)->x;
        }
    }

    vector<QCustomPlot *> errorCanvas(split - 1);
    for (qint32 i = 0; i < split - 1; ++i) {
        errorCanvas[i] = new QCustomPlot;
        errorCanvas.at(i)->addGraph();
        errorCanvas.at(i)->graph(0)->setPen(QPen(QColor(250, 128, 114)));
        errorCanvas.at(i)->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
        errorCanvas.at(i)->plotLayout()->insertRow(0);
        errorCanvas.at(i)->plotLayout()->addElement(0, 0, new QCPTextElement(errorCanvas.at(i), "水平方向" + QString::number(i + 1) + "/" + QString::number(split) + "处误差图", QFont(font().family(), 10, QFont::Bold)));
        errorCanvas.at(i)->xAxis->setVisible(true);
        errorCanvas.at(i)->xAxis->setTickLabels(true);
        errorCanvas.at(i)->xAxis->setLabel(tr("水平方向"));
        errorCanvas.at(i)->yAxis->setVisible(true);
        errorCanvas.at(i)->yAxis->setTickLabels(true);
        errorCanvas.at(i)->yAxis->setLabel(tr("误差(m)"));

        double x_flag = x.at(x.size() / split * (i + 1));
        bool is_first_find = true;
        int count = 0;
        for (size_t k = 0; k < gEstimatorsAccordingToX.size(); ++k) {
            if (x_flag == gEstimatorsAccordingToX.at(k)->x) {
                errorCanvas.at(i)->graph(0)->addData(count, gEstimatorsAccordingToX.at(k)->err);  // 实时绘图
                count++;
                is_first_find = false;
            } else {
                if (!is_first_find) {
                    break;
                }
            }
        }
        errorCanvas.at(i)->xAxis->setRange(0, count);
        errorCanvas.at(i)->yAxis->setRange(0, 0.1);
        errorCanvas.at(i)->replot();

        errorCanvas.at(i)->saveJpg("D:/我的文档/Qt/水平方向" + QString::number(i + 1) + "_" + QString::number(split) + "处误差图.jpg", 488, 485, 1, -1);  // TODO
    }

    for (size_t i = 0; i < errorCanvas.size(); ++i) {
        delete errorCanvas.at(i);
    }
}

void FourthWindow::PlotY(qint32 split) {
    vector<double> y;
    double cur_y = gEstimatorsAccordingToY.at(0)->y;
    y.push_back(cur_y);
    for (size_t i = 0; i < gEstimatorsAccordingToY.size(); ++i) {
        if (cur_y != gEstimatorsAccordingToY.at(i)->y) {
            y.push_back(cur_y);
            cur_y = gEstimatorsAccordingToY.at(i)->y;
        }
    }

    vector<QCustomPlot *> errorCanvas(split - 1);
    for (qint32 i = 0; i < split - 1; ++i) {
        errorCanvas[i] = new QCustomPlot;
        errorCanvas.at(i)->addGraph();
        errorCanvas.at(i)->graph(0)->setPen(QPen(QColor(250, 128, 114)));
        errorCanvas.at(i)->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
        errorCanvas.at(i)->plotLayout()->insertRow(0);
        errorCanvas.at(i)->plotLayout()->addElement(0, 0, new QCPTextElement(errorCanvas.at(i), "垂直方向" + QString::number(i + 1) + "/" + QString::number(split) + "处误差图", QFont(font().family(), 10, QFont::Bold)));
        errorCanvas.at(i)->xAxis->setVisible(true);
        errorCanvas.at(i)->xAxis->setTickLabels(true);
        errorCanvas.at(i)->xAxis->setLabel(tr("垂直方向"));
        errorCanvas.at(i)->yAxis->setVisible(true);
        errorCanvas.at(i)->yAxis->setTickLabels(true);
        errorCanvas.at(i)->yAxis->setLabel(tr("误差(m)"));

        double y_flag = y.at(y.size() / split * (i + 1));
        bool is_first_find = true;
        int count = 0;
        for (size_t k = 0; k < gEstimatorsAccordingToY.size(); ++k) {
            if (y_flag == gEstimatorsAccordingToY.at(k)->y) {
                errorCanvas.at(i)->graph(0)->addData(count, gEstimatorsAccordingToY.at(k)->err);  // 实时绘图
                count++;
                is_first_find = false;
            } else {
                if (!is_first_find) {
                    break;
                }
            }
        }
        errorCanvas.at(i)->xAxis->setRange(0, count);
        errorCanvas.at(i)->yAxis->setRange(0, 0.1);
        errorCanvas.at(i)->replot();

        errorCanvas.at(i)->saveJpg("D:/我的文档/Qt/垂直方向" + QString::number(i + 1) + "_" + QString::number(split) + "处误差图.jpg", 488, 485, 1, -1);  // TODO
    }

    for (size_t i = 0; i < errorCanvas.size(); ++i) {
        delete errorCanvas.at(i);
    }
}


void FourthWindow::PlotHeatMap() {
}

void FourthWindow::SlotLeftArrowClicked() {
    if (gStackWinIndex == 0) {
        gStackWinIndex = 2;
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
    if (gStackWinIndex == 2) {
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

void FourthWindow::SlotEstimate() {
    if (gXDivide->text() != "" && gYDivide->text() != "") {
        PlotX(gXDivide->text().toUInt());
        PlotY(gYDivide->text().toUInt());
    }
}
