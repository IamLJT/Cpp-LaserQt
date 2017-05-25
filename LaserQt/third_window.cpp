#include "third_window.h"

ThirdWindow::ThirdWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

ThirdWindow::~ThirdWindow() {
    delete gObjectDataFile;
    delete gScanningDataFile;
    delete gLogger;
    delete gPointCloudDataScanningButton;
    delete gPointCloudDataDenoisingButton;
    delete gPointCloudDataFittingButton;
    delete gExecuteProgressBar;
}

void ThirdWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}

void ThirdWindow::SetWidgets() {
    /* left-top layout */
    QGridLayout * leftTopLayout = new QGridLayout;
    leftTopLayout->setSpacing(20);

    leftTopLayout->addWidget(new QLabel(tr("目标数据")), 0, 0);
    gObjectDataFile = new QLineEdit;
    leftTopLayout->addWidget(gObjectDataFile, 0, 1);
    QPushButton * browseObjectDataFileButton = new QPushButton(tr("浏览"));
    connect(browseObjectDataFileButton, SIGNAL(clicked()), this, SLOT(SlotOpenObjectDataFile()));
    leftTopLayout->addWidget(browseObjectDataFileButton, 0, 2);

    leftTopLayout->addWidget(new QLabel(tr("扫描编号")), 1, 0);
    gScanningDataFile = new QLineEdit;
    leftTopLayout->addWidget(gScanningDataFile, 1, 1);
    QPushButton * browseScanningDataFileButton = new QPushButton(tr("浏览"));
    connect(browseScanningDataFileButton, SIGNAL(clicked()), this, SLOT(SlotOpenScanningDataFile()));
    leftTopLayout->addWidget(browseScanningDataFileButton, 1, 2);

    /* left-middle layout */
    gLogger = new QTextEdit;
    // gLogger->setEnabled(false);
    gExecuteProgressBar = new QProgressBar;

    /* left-bottom layout */
    gPointCloudDataScanningButton = new QPushButton(tr("点云扫描"));
    gPointCloudDataScanningButton->setEnabled(false);
    connect(gPointCloudDataScanningButton, SIGNAL(clicked()), this, SLOT(SlotScanPointCloudData()));
    gPointCloudDataDenoisingButton = new QPushButton(tr("点云去噪"));
    gPointCloudDataDenoisingButton->setEnabled(false);
    connect(gPointCloudDataDenoisingButton, SIGNAL(clicked()), this, SLOT(SlotDenoisePointCloudData()));
    gPointCloudDataFittingButton = new QPushButton(tr("点云拟合"));
    gPointCloudDataFittingButton->setEnabled(false);
    connect(gPointCloudDataFittingButton, SIGNAL(clicked()), this, SLOT(SlotFitPointCloudData()));

    QHBoxLayout * leftBottomLayout = new QHBoxLayout;
    leftBottomLayout->addStretch();
    leftBottomLayout->setSpacing(20);
    leftBottomLayout->addWidget(gPointCloudDataScanningButton);
    leftBottomLayout->addWidget(gPointCloudDataDenoisingButton);
    leftBottomLayout->addWidget(gPointCloudDataFittingButton);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(gLogger);
    leftLayout->addWidget(gExecuteProgressBar);
    leftLayout->addSpacing(30);
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

void ThirdWindow::SlotOpenObjectDataFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QString::fromStdString(GetWorkDirectory()), tr("Text File(*.txt)"));
    if (filePath != "") {
        gObjectDataFile->setText(filePath);
    }
}

void ThirdWindow::SlotOpenScanningDataFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QString::fromStdString(GetWorkDirectory()), tr("Text File(*.txt)"));
    if (filePath != "") {
        gScanningDataFile->setText(filePath);
        gPointCloudDataScanningButton->setEnabled(true);
    }
}

void ThirdWindow::SlotScanPointCloudData() {
    gPointCloudDataDenoisingButton->setEnabled(true);
}

void ThirdWindow::SlotDenoisePointCloudData() {
    gPointCloudDataFittingButton->setEnabled(true);
}

void ThirdWindow::SlotFitPointCloudData() {}
