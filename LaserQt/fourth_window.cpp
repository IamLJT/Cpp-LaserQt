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
    delete gXStart;
    delete gYStart;
    delete gXEnd;
    delete gYEnd;
}

void FourthWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}

void FourthWindow::SetWidgets() {
    /* left-top layout */
    QGridLayout * leftTopLayout = new QGridLayout;

    gErrorCanvas_1 = new QCustomPlot;
    gErrorCanvas_1->addGraph();
    gErrorCanvas_1->plotLayout()->insertRow(0);
    gErrorCanvas_1->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_1, tr("加工板水平方向1/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_1->xAxis->setVisible(true);
    gErrorCanvas_1->xAxis->setTickLabels(false);
    gErrorCanvas_1->yAxis->setVisible(true);
    gErrorCanvas_1->yAxis->setTickLabels(false);
    leftTopLayout->addWidget(gErrorCanvas_1, 0, 0);
    gErrorCanvas_2 = new QCustomPlot;
    gErrorCanvas_2->addGraph();
    gErrorCanvas_2->plotLayout()->insertRow(0);
    gErrorCanvas_2->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_2, tr("加工板水平方向1/2处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_2->xAxis->setVisible(true);
    gErrorCanvas_2->xAxis->setTickLabels(false);
    gErrorCanvas_2->yAxis->setVisible(true);
    gErrorCanvas_2->yAxis->setTickLabels(false);
    leftTopLayout->addWidget(gErrorCanvas_2, 0, 1);
    gErrorCanvas_3 = new QCustomPlot;
    gErrorCanvas_3->addGraph();
    gErrorCanvas_3->plotLayout()->insertRow(0);
    gErrorCanvas_3->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_3, tr("加工板水平方向2/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_3->xAxis->setVisible(true);
    gErrorCanvas_3->xAxis->setTickLabels(false);
    gErrorCanvas_3->yAxis->setVisible(true);
    gErrorCanvas_3->yAxis->setTickLabels(false);
    leftTopLayout->addWidget(gErrorCanvas_3, 0, 2);
    gErrorCanvas_4 = new QCustomPlot;
    gErrorCanvas_4->addGraph();
    gErrorCanvas_4->plotLayout()->insertRow(0);
    gErrorCanvas_4->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_4, tr("加工板垂直方向1/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_4->xAxis->setVisible(true);
    gErrorCanvas_4->xAxis->setTickLabels(false);
    gErrorCanvas_4->yAxis->setVisible(true);
    gErrorCanvas_4->yAxis->setTickLabels(false);
    leftTopLayout->addWidget(gErrorCanvas_4, 1, 0);
    gErrorCanvas_5 = new QCustomPlot;
    gErrorCanvas_5->addGraph();
    gErrorCanvas_5->plotLayout()->insertRow(0);
    gErrorCanvas_5->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_5, tr("加工板垂直方向2/3处误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_5->xAxis->setVisible(true);
    gErrorCanvas_5->xAxis->setTickLabels(false);
    gErrorCanvas_5->yAxis->setVisible(true);
    gErrorCanvas_5->yAxis->setTickLabels(false);
    leftTopLayout->addWidget(gErrorCanvas_5, 1, 1);
    gErrorCanvas_6 = new QCustomPlot;
    gErrorCanvas_6->addGraph();
    gErrorCanvas_6->plotLayout()->insertRow(0);
    gErrorCanvas_6->plotLayout()->addElement(0, 0, new QCPTextElement(gErrorCanvas_6, tr("加工板任意两点间误差曲线图"), QFont(font().family(), 10, QFont::Bold)));
    gErrorCanvas_6->xAxis->setVisible(true);
    gErrorCanvas_6->xAxis->setTickLabels(false);
    gErrorCanvas_6->yAxis->setVisible(true);
    gErrorCanvas_6->yAxis->setTickLabels(false);
    leftTopLayout->addWidget(gErrorCanvas_6, 1, 2);


    /* left-middle layout */
    QGridLayout * leftMiddleLayout = new QGridLayout;
    leftMiddleLayout->setSpacing(20);

    leftMiddleLayout->addWidget(new QLabel(tr("起点Ｘ坐标")), 0, 0);
    gXStart = new QLineEdit;
    leftMiddleLayout->addWidget(gXStart, 0, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("起点Y坐标")), 0, 2);
    gYStart = new QLineEdit;
    leftMiddleLayout->addWidget(gYStart, 0, 3);
    leftMiddleLayout->addWidget(new QLabel(tr("终点Ｘ坐标")), 1, 0);
    gXEnd = new QLineEdit;
    leftMiddleLayout->addWidget(gXEnd, 1, 1);
    leftMiddleLayout->addWidget(new QLabel(tr("终点Y坐标")), 1, 2);
    gYEnd = new QLineEdit;
    leftMiddleLayout->addWidget(gYEnd, 1, 3);

    /* left-bottom layout */
    QPushButton * okButton = new QPushButton(tr("确定"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(SlotOK()));
    QPushButton * enlargeButton = new QPushButton(tr("放大显示"));
    connect(enlargeButton, SIGNAL(clicked()), this, SLOT(SlotEnlarge()));

    QHBoxLayout * leftBottomLayout = new QHBoxLayout;
    leftBottomLayout->addStretch();
    leftBottomLayout->setSpacing(20);
    leftBottomLayout->addWidget(okButton);
    leftBottomLayout->addWidget(enlargeButton);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(30);
    leftLayout->addLayout(leftMiddleLayout);
    leftLayout->addSpacing(10);
    leftLayout->addLayout(leftBottomLayout);

    /* right layout */
    QVBoxLayout * rightLayout = new QVBoxLayout;

    /* main layout */
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(leftLayout, 1);
    layout->setStretchFactor(rightLayout, 1);

    setLayout(layout);
}

void FourthWindow::SlotOK() {}

void FourthWindow::SlotEnlarge() {}
