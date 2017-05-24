#include "second_window.h"

SecondWindow::SecondWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

SecondWindow::~SecondWindow() {
    delete gTaskQueue;
}

void SecondWindow::clear() {
    gInputFile->clear();
    gPathIndex->clear();
    gStartCoord->clear();
    gEndCoord->clear();
    gReduction->clear();
    gThermalParameter->clear();
    gProcessingTime->clear();
    while (!gTaskQueue->empty()) {
        gTaskQueue->dequeue();
    }
    gLogger->clear();
    gStartProcessingButton->setEnabled(false);
    gStopProcessingButton->setEnabled(false);
    gContinueProcessingButton->setEnabled(false);
    gCustomPlot->clearItems();
}

void SecondWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}

void SecondWindow::SetWidgets() {
    /* left-top layout */
    QGridLayout * leftTopLayout = new QGridLayout;
    leftTopLayout->setSpacing(25);

    leftTopLayout->addWidget(new QLabel(tr("加载文件")), 0, 0);
    gInputFile = new QLineEdit;
    leftTopLayout->addWidget(gInputFile, 0, 1);
    QPushButton * browseButton = new QPushButton(tr("浏览"));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(SlotOpenFile()));
    leftTopLayout->addWidget(browseButton, 0, 2);

    leftTopLayout->addWidget(new QLabel(tr("路径编号")), 1, 0);
    gPathIndex = new QLineEdit;
    gPathIndex->setEnabled(false);
    leftTopLayout->addWidget(gPathIndex, 1, 1);

    leftTopLayout->addWidget(new QLabel(tr("起点坐标")), 2, 0);
    gStartCoord = new QLineEdit;
    gStartCoord->setEnabled(false);
    leftTopLayout->addWidget(gStartCoord, 2, 1);

    leftTopLayout->addWidget(new QLabel(tr("终点坐标")), 3, 0);
    gEndCoord = new QLineEdit;
    gEndCoord->setEnabled(false);
    leftTopLayout->addWidget(gEndCoord, 3, 1);

    leftTopLayout->addWidget(new QLabel(tr("下压量")), 4, 0);
    gReduction = new QLineEdit;
    gReduction->setEnabled(false);
    leftTopLayout->addWidget(gReduction, 4, 1);

    leftTopLayout->addWidget(new QLabel(tr("热参数")), 5, 0);
    gThermalParameter = new QLineEdit;
    gThermalParameter->setEnabled(false);
    leftTopLayout->addWidget(gThermalParameter, 5, 1);

    leftTopLayout->addWidget(new QLabel(tr("加工时间")), 6, 0);
    gProcessingTime = new QLineEdit;
    gProcessingTime->setEnabled(false);
    leftTopLayout->addWidget(gProcessingTime, 6, 1);

    gTaskQueue = new QQueue<QVector<double>>;

    /* left-middle layout */
    gLogger = new QTextEdit;
    gLogger->setEnabled(false);

    /* left-bottom layout */
    gStartProcessingButton = new QPushButton(tr("开始加工"));
    gStartProcessingButton->setEnabled(false);
    connect(gStartProcessingButton, SIGNAL(clicked()), this, SLOT(SlotStartProcessing()));
    gStopProcessingButton = new QPushButton(tr("停止加工"));
    gStopProcessingButton->setEnabled(false);
    connect(gStopProcessingButton, SIGNAL(clicked()), this, SLOT(SlotStopProcessing()));
    gContinueProcessingButton = new QPushButton(tr("继续加工"));
    gContinueProcessingButton->setEnabled(false);
    connect(gContinueProcessingButton, SIGNAL(clicked()), this, SLOT(SlotContinueProcessing()));

    QHBoxLayout * leftBottomLayout = new QHBoxLayout;
    leftBottomLayout->addStretch();
    leftBottomLayout->setSpacing(20);
    leftBottomLayout->addWidget(gStartProcessingButton);
    leftBottomLayout->addWidget(gStopProcessingButton);
    leftBottomLayout->addWidget(gContinueProcessingButton);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addStretch();
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(gLogger);
    leftLayout->addSpacing(30);
    leftLayout->addLayout(leftBottomLayout);

    /* right layout */
    gCustomPlot = new QCustomPlot;
    gCustomPlot->addGraph();
    gCustomPlot->plotLayout()->insertRow(0);
    gCustomPlot->plotLayout()->addElement(0, 0, new QCPTextElement(gCustomPlot, tr("加工路径动态图"), QFont(font().family(), 12, QFont::Bold)));
    gCustomPlot->xAxis->setLabel(tr("X-板长方向(m)"));
    gCustomPlot->xAxis->setVisible(true);
    gCustomPlot->xAxis->setTickLabels(false);
    gCustomPlot->yAxis->setLabel(tr("Y-板宽方向(m)"));
    gCustomPlot->yAxis->setVisible(true);
    gCustomPlot->yAxis->setTickLabels(false);

    QVBoxLayout * rightLayout = new QVBoxLayout;
    rightLayout->addWidget(gCustomPlot);

    /* main layout */
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(leftLayout, 1);
    layout->setStretchFactor(rightLayout, 1);

    setLayout(layout);
}

void SecondWindow::InitTaskQueue() {
    using namespace libxl;
    Book * book = xlCreateXMLBook();
    // Book * book = xlCreateBook();  // xlCreateBook() for xls
    if (book) {
        if (book->load(gInputFile->text().toStdString().c_str())) {
            Sheet * sheet = book->getSheet(0);
            if (sheet) {
                for (int i = 1; i < sheet->lastRow(); ++i) {
                    QVector<double> dataCell;
                    dataCell.push_back(double(i));
                    for (int j = 0; j < 7; ++j) {
                        dataCell.push_back(sheet->readNum(i, j));
                    }
                    gTaskQueue->enqueue(dataCell);
                }
            }
            gLogger->append(tr("[+] 初始化任务队列完毕。"));
            gLogger->append(tr("[+] 新加入")+QString::number(sheet->lastRow())+tr("项任务。"));
            book->release();
            gStartProcessingButton->setEnabled(true);
        }
    }
}

void SecondWindow::SlotOpenFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QString::fromStdString(GetWorkDirectory()), tr("Excel File(*.xlsx)"));
    if (filePath != "") {
        gInputFile->setText(filePath);
        InitTaskQueue();
    }
}

void SecondWindow::SlotStartProcessing() {
    gStopProcessingButton->setEnabled(true);
    while (!gTaskQueue->empty()) {
        QVector<double> dataCell = gTaskQueue->dequeue();
        gPathIndex->setText(QString::number(dataCell.at(0)));
        gStartCoord->setText("(" + QString::number(dataCell.at(1)) + ", " + QString::number(dataCell.at(2)) + ")");
        gEndCoord->setText("(" + QString::number(dataCell.at(3)) + ", " + QString::number(dataCell.at(4)) + ")");
        gReduction->setText(QString::number(dataCell.at(5)));
        gThermalParameter->setText(QString::number(dataCell.at(6)));
        Sleep(1000);
    }
}

void SecondWindow::SlotStopProcessing() {
    gContinueProcessingButton->setEnabled(true);
}

void SecondWindow::SlotContinueProcessing() {}

void SecondWindow::Sleep(size_t msec) {
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
