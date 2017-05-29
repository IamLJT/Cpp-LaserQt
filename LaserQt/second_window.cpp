#include "second_window.h"
#include "QDebug"

SecondWindow::SecondWindow(QWidget *parent) :
    QWidget(parent) {
    CreateMainWindow();
}

SecondWindow::~SecondWindow() {
    delete gInputFile;
    delete gPathIndex;
    delete gStartCoord;
    delete gEndCoord;
    delete gReduction;
    delete gThermalParameter;
    delete gProcessingTime;
    delete gStartProcessingButton;
    delete gStopProcessingButton;
    delete gContinueProcessingButton;
    delete gCustomPlot;
    delete gLogger;
    delete gTaskQueue;
    delete gTaskList;
    delete gTimer;
}

void SecondWindow::clear() {
    gInputFile->clear();
    gPathIndex->clear();
    gStartCoord->clear();
    gEndCoord->clear();
    gReduction->clear();
    gThermalParameter->clear();
    gProcessingTime->setText("00:00:00");
    while (!gTaskQueue->empty()) {
        gTaskQueue->dequeue();
    }
    gLogger->clear();
    gStartProcessingButton->setEnabled(false);
    gStopProcessingButton->setEnabled(false);
    gContinueProcessingButton->setEnabled(false);
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
    gProcessingTime->setText("00:00:00");
    leftTopLayout->addWidget(gProcessingTime, 6, 1);

    /* 任务队列 */
    gTaskQueue = new QQueue<QVector<double>>;
    gTaskList = new QVector<QVector<double>>;
    /* 计时器 */
    gTimer = new QTimer;
    connect(gTimer, SIGNAL(timeout()), this, SLOT(SlotUpdateTime()));
    gCounter = 0;
    gIsStop = false;

    /* left-middle layout */
    gLogger = new QTextEdit;
    // gLogger->setEnabled(false);

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
    gCustomPlot->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    gCustomPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    gCustomPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));
    gCustomPlot->plotLayout()->insertRow(0);
    gCustomPlot->plotLayout()->addElement(0, 0, new QCPTextElement(gCustomPlot, tr("加工路径动态图"), QFont(font().family(), 12, QFont::Bold)));
    gCustomPlot->xAxis->setLabel(tr("X-板长方向(m)"));
    gCustomPlot->xAxis->setVisible(true);
    gCustomPlot->xAxis->setTickLabels(true);
    gCustomPlot->yAxis->setLabel(tr("Y-板宽方向(m)"));
    gCustomPlot->yAxis->setVisible(true);
    gCustomPlot->yAxis->setTickLabels(true);

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
#ifdef __linux__
    using namespace libxl;
    Book * book = xlCreateXMLBook();
    // Book * book = xlCreateBook();  // xlCreateBook() for xls
    book->setKey(L"Michael Hillier", L"windows-222329040ec5ec046fb46767a7h1gej6");
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
                    gTaskList->push_back(dataCell);
                }
            }
            gLogger->append(tr("[+] 初始化任务队列完毕."));
            gLogger->append(tr("[+] 新加入")+QString::number(sheet->lastRow() - 1)+tr("项任务."));
            book->release();
            gStartProcessingButton->setEnabled(true);
        }
    }
#elif _WIN32
    QXlsx::Document xlsx(gInputFile->text());
    for (int i = 2; i <= xlsx.dimension().lastRow(); ++i) {
        QVector<double> dataCell;
        dataCell.push_back(double(i - 1));
        for (int j = 1; j <= xlsx.dimension().lastColumn(); ++j) {
            if (QXlsx::Cell * cell = xlsx.cellAt(i, j)) {
                dataCell.push_back(cell->value().toDouble());
            }
            gTaskQueue->enqueue(dataCell);
            gTaskList->push_back(dataCell);
        }
    }
    gLogger->append(tr("[+] 初始化任务队列完毕."));
    gLogger->append(tr("[+] 新加入")+QString::number(xlsx.dimension().lastRow() - 1)+tr("项任务."));
    gStartProcessingButton->setEnabled(true);
#endif
}

void SecondWindow::Sleep(size_t msec) {
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void SecondWindow::InitSocket() {
    gUDPSocket = new QUdpSocket;
    hostAddress.setAddress(gProcessMachineIP);
    gUDPSocket->bind(hostAddress, gProcessMachinePort);
    connect(gUDPSocket, SIGNAL(readyRead()), this, SLOT(SlotReadPendingDatagrams()));
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

    QFile fd(":/xml/xml/config.xml");
    if (fd.open(QFile::ReadOnly | QFile::Text)) {
        QXmlStreamReader * reader = new QXmlStreamReader(&fd);
        while (!reader->atEnd()) {
            reader->readNext();
            if (reader->name() == "hostname") {
                gProcessMachineIP = reader->readElementText();
            }
            else if (reader->name() == "port") {
                gProcessMachinePort = reader->readElementText().toUShort();
            }
        }
        delete reader;
        fd.close();
    }

    QString processMachine = gProcessMachineIP + ":" + QString::number(gProcessMachinePort);
    InitSocket();  // 初始化Socket

    MyMessageBox msgBox;
    msgBox.setText(tr("主工控机: <") + processMachine + ">");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    if (msgBox.exec() == QMessageBox::Save) {
        while (!gTaskQueue->empty()) {
            QVector<double> dataCell = gTaskQueue->dequeue();
            QVariantList array;
            array << int(dataCell.at(0)) << dataCell.at(1) << dataCell.at(2) << dataCell.at(3);
            array << dataCell.at(4) << int(dataCell.at(5)) << int(dataCell.at(6)) << int(dataCell.at(7));

            QJson::Serializer serializer;
            bool ok;
            QByteArray json = serializer.serialize(array, &ok);
            if (ok) {
                gUDPSocket->writeDatagram(json.data(), json.size(), hostAddress, gProcessMachinePort);  // 发送数据包
            }

            Sleep(10);  // TODO
        }
        gTimer->start(1000);  // the UpdateTime() slot is called every second.
    }
}

void SecondWindow::SlotStopProcessing() {
    gContinueProcessingButton->setEnabled(true);
    gIsStop = true;
}

void SecondWindow::SlotContinueProcessing() {
    gIsStop = false;
}

void SecondWindow::SlotUpdateTime() {
    // QDateTime time = QDateTime::currentDateTime();
    // gProcessingTime->setText(time.toString("hh:mm:ss"));
    gCounter++;
    int h = gCounter / 3600;
    int m = (gCounter % 3600) / 60;
    int s = gCounter % 60;

    QString h_str;
    if (h < 10) {
        h_str = "0" + QString::number(h);
    } else {
        h_str = QString::number(h);
    }
    QString m_str;
    if (m < 10) {
        m_str = "0" + QString::number(m);
    } else {
        m_str = QString::number(m);
    }
    QString s_str;
    if (s < 10) {
        s_str = "0" + QString::number(s);
    } else {
        s_str = QString::number(s);
    }

    gProcessingTime->setText(h_str + ":" + m_str + ":" + s_str);
}

void SecondWindow::SlotReadPendingDatagrams() {
    static int pathIndex_ = 0;
    while (gUDPSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(gUDPSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        gUDPSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);  // TODO

        QJson::Parser parser;
        bool ok;
        QVariantList data = parser.parse (datagram, &ok).toList();
        double x = data[0].toDouble();
        double y = data[1].toDouble();
        int pathIndex = data[2].toInt();
        // int flag = data[3].toInt();
        if (pathIndex == gTaskList->size() + 1) {
            gLogger->append(tr("[+] 第") + QString::number(pathIndex - 1) + ("项任务处理完毕."));
            gTimer->stop();
        }
        else {
            if (pathIndex != pathIndex_) {
                pathIndex_ = pathIndex;
                QVector<double> dataCell = gTaskList->at(pathIndex_ - 1);
                if (pathIndex_ == 1) {
                    gLogger->append(tr("[+] 正在处理第") + QString::number(dataCell.at(0)) + ("项任务..."));
                } else {
                    gLogger->append(tr("[+] 第") + QString::number(dataCell.at(0) - 1) + ("项任务处理完毕."));
                    gLogger->append(tr("[+] 正在处理第") + QString::number(dataCell.at(0)) + ("项任务..."));
                }
                gPathIndex->setText(QString::number(dataCell.at(0)));
                gStartCoord->setText("(" + QString::number(dataCell.at(1)) + ", " + QString::number(dataCell.at(2)) + ")");
                gEndCoord->setText("(" + QString::number(dataCell.at(3)) + ", " + QString::number(dataCell.at(4)) + ")");
                gReduction->setText(QString::number(dataCell.at(5)));
                gThermalParameter->setText(QString::number(dataCell.at(6)));
            }
            gCustomPlot->graph(0)->addData(x, y);
            gCustomPlot->graph(0)->rescaleAxes();
            gCustomPlot->replot();
        }
    }
}
