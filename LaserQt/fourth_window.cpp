#include "fourth_window.h"

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
    delete gLeftArrowLabel;
    delete gRightArrowLabel;
    delete gStackWin;
    delete gXStart;
    delete gYStart;
    delete gXEnd;
    delete gYEnd;
    delete gOKButton;
    delete gPointCloudDataGraph;
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

    gStackWin = new QStackedWidget;
    gStackWin->addWidget(gErrorCanvas_1);
    gStackWin->addWidget(gErrorCanvas_2);
    gStackWin->addWidget(gErrorCanvas_3);
    gStackWin->addWidget(gErrorCanvas_4);
    gStackWin->addWidget(gErrorCanvas_5);
    gStackWin->addWidget(gErrorCanvas_6);
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

void FourthWindow::SlotLeftArrowClicked() {
    if (gStackWinIndex == 0) {
        gStackWinIndex = 5;
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
    if (gStackWinIndex == 5) {
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
