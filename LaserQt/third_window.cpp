#include "third_window.h"

ThirdWindow::ThirdWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

ThirdWindow::~ThirdWindow() {
    delete gObjectDataFile;
    delete gScanningDataFile;
    delete gLogger;
    delete gExecuteProgressBar;
    delete gCameraPresentList;
    delete gPointCloudDataScanningButton;
    delete gPointCloudDataDenoisingButton;
    delete gPointCloudDataFittingButton;
    delete gPointCloudDataGraph;
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

    leftTopLayout->addWidget(new QLabel(tr("扫描数据")), 1, 0);
    gScanningDataFile = new QLineEdit;
    leftTopLayout->addWidget(gScanningDataFile, 1, 1);
    QPushButton * browseScanningDataFileButton = new QPushButton(tr("浏览"));
    connect(browseScanningDataFileButton, SIGNAL(clicked()), this, SLOT(SlotOpenScanningDataFile()));
    leftTopLayout->addWidget(browseScanningDataFileButton, 1, 2);

    /* left-middle layout */
    gLogger = new QTextEdit;
    // gLogger->setEnabled(false);
    gExecuteProgressBar = new QProgressBar;

    /* left-middle layout */
    gCameraPresentList = new QComboBox;
    gCameraPresentList->addItem(QStringLiteral("前视"));
    gCameraPresentList->addItem(QStringLiteral("后视"));
    gCameraPresentList->addItem(QStringLiteral("上视"));
    gCameraPresentList->addItem(QStringLiteral("下视"));
    gCameraPresentList->addItem(QStringLiteral("左视"));
    gCameraPresentList->addItem(QStringLiteral("右视"));
    gCameraPresentList->addItem(QStringLiteral("正向45°"));
    gCameraPresentList->addItem(QStringLiteral("反向45°"));
    gCameraPresentList->addItem(QStringLiteral("等距左视"));
    gCameraPresentList->addItem(QStringLiteral("等距右视"));
    gCameraPresentList->setCurrentIndex(9);

    QPushButton * cameraPresentButton = new QPushButton(tr("确定"));
    connect(cameraPresentButton, SIGNAL(clicked()), this, SLOT(SlotAdjustCameraPresent()));

    QHBoxLayout * leftMiddleLayout = new QHBoxLayout;
    leftMiddleLayout->addStretch();
    leftMiddleLayout->setSpacing(20);
    leftMiddleLayout->addWidget(new QLabel(tr("三维场观察视角：")));
    leftMiddleLayout->addWidget(gCameraPresentList);
    leftMiddleLayout->addWidget(cameraPresentButton);

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
    leftLayout->addSpacing(20);
    leftLayout->addWidget(gExecuteProgressBar);
    leftLayout->addSpacing(10);
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

QString ThirdWindow::CopyObjectDataFilePath() {
    return gObjectDataFile->text();
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
        gPointCloudDataFittingButton->setEnabled(true);

#ifdef _WIN32
        QProcess * proc = new QProcess;
        proc->setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments * args) {
            args->startupInfo->wShowWindow |= SW_SHOW;
            args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        });

        proc->start("D:\\Cpp-LaserQt\\cmake-gui.exe", QStringList() << "");
        if (proc->waitForStarted()) {
            proc->waitForFinished();
            MyMessageBox msgBox;
            msgBox.setText(tr("点云扫描完毕!"));
            msgBox.setStandardButtons(QMessageBox::Save);
            if (QProcess::NormalExit == proc->exitCode()) {
                MyMessageBox msgBox;
                msgBox.setText(tr("点云扫描完毕!"));
                msgBox.setStandardButtons(QMessageBox::Save);
            }
        } else {
            MyMessageBox msgBox;
            msgBox.setText(tr("调用外部程序失败!"));
            msgBox.setStandardButtons(QMessageBox::Save);
        }
        delete proc;
        proc = NULL;
#endif
    }
}

void ThirdWindow::SlotScanPointCloudData() {
    gPointCloudDataDenoisingButton->setEnabled(true);
}

void ThirdWindow::SlotDenoisePointCloudData() {

}

void ThirdWindow::SlotFitPointCloudData() {
    QtDataVisualization::QScatter3DSeries * series1 = new QtDataVisualization::QScatter3DSeries;
    series1->setMeshSmooth(QtDataVisualization::QAbstract3DSeries::MeshPoint);
    QtDataVisualization::QScatterDataArray data1;
    QFile f1(gObjectDataFile->text());
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
    QFile f2(gScanningDataFile->text());
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
}

void ThirdWindow::SlotAdjustCameraPresent() {
    int index = gCameraPresentList->currentIndex();
    switch (index) {
    case 0:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetFront);
        break;
    case 1:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetBehind);
        break;
    case 2:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetDirectlyAbove);
        break;
    case 3:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetDirectlyBelow);
        break;
    case 4:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetLeft);
        break;
    case 5:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetRight);
        break;
    case 6:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetDirectlyAboveCW45);
        break;
    case 7:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetDirectlyAboveCCW45);
        break;
    case 8:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetIsometricLeft);
        break;
    case 9:
        gPointCloudDataGraph->scene()->activeCamera()->setCameraPreset(QtDataVisualization::Q3DCamera::CameraPresetIsometricRight);
        break;
    }
}
