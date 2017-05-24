#include "main_window.h"

MainWindow::MainWindow(QWidget * parent) :
    QWidget(parent) {
    CreateMainWindow();
}

MainWindow::~MainWindow() {

}

void MainWindow::CreateMainWindow() {
    setMinimumSize(GetScreenSize().first, GetScreenSize().second);
    setMaximumSize(GetScreenSize().first, GetScreenSize().second);

    SetWidgets();
}

void MainWindow::SetWidgets() {
    /* left-top layout */
    gInputFile = new QLineEdit;
    QPushButton * browseButton = new QPushButton(tr("浏览"));
    // browseButton->setIcon(QPixmap::fromImage(QImage(":/img/ui/browse.png")));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(SlotOpenFile()));

    QHBoxLayout * leftTopLayout = new QHBoxLayout;
    leftTopLayout->setSpacing(20);
    leftTopLayout->addWidget(gInputFile);
    leftTopLayout->addWidget(browseButton);

    /* left-middle layout */
    gDataTable = new QTableWidget(0, 7);
    QStringList headers;
    headers.append(tr("起点X坐标"));
    headers.append(tr("起点Y坐标"));
    headers.append(tr("终点X坐标"));
    headers.append(tr("终点Y坐标"));
    headers.append(tr("下压量"));
    headers.append(tr("热参数"));
    headers.append(tr("正反标志"));
    gDataTable->setHorizontalHeaderLabels(headers);
    gDataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gDataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gEditFlag = false;

    /* left-bottom layout */
    gEditButton = new QPushButton(tr("开启编辑"));
    connect(gEditButton, SIGNAL(clicked()), this, SLOT(SlotEditTable()));
    QPushButton * updateButton = new QPushButton(tr("更新表格"));
    connect(updateButton, SIGNAL(clicked()), this, SLOT(SlotUpdateTable()));
    QPushButton * nextButton = new QPushButton(tr("下一步"));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(SlotNext()));
    QPushButton * quitButton = new QPushButton(tr("退出"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(SlotQuit()));

    QHBoxLayout * leftBottomLayout = new QHBoxLayout;
    leftBottomLayout->addStretch();
    leftBottomLayout->addWidget(gEditButton);
    leftBottomLayout->addSpacing(40);
    leftBottomLayout->addWidget(updateButton);
    leftBottomLayout->addSpacing(80);
    leftBottomLayout->addWidget(nextButton);
    leftBottomLayout->addSpacing(40);
    leftBottomLayout->addWidget(quitButton);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(gDataTable);
    leftLayout->addSpacing(20);
    leftLayout->addLayout(leftBottomLayout);

    /* right layout */
    gCustomPlot = new QCustomPlot;
    gCustomPlot->addGraph();
    gCustomPlot->plotLayout()->insertRow(0);
    gCustomPlot->plotLayout()->addElement(0, 0, new QCPTextElement(gCustomPlot, tr("加工路径静态图"), QFont(font().family(), 12, QFont::Bold)));  // TODO
    gCustomPlot->xAxis->setLabel(tr("X-板长方向(m)"));
    gCustomPlot->xAxis->setVisible(true);
    gCustomPlot->xAxis->setTickLabels(false);
    gCustomPlot->yAxis->setLabel(tr("Y-板宽方向(m)"));
    gCustomPlot->yAxis->setVisible(true);
    gCustomPlot->yAxis->setTickLabels(false);

    QVBoxLayout * rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0, 44, 0, 54);
    rightLayout->addWidget(gCustomPlot);

    /* main layout */
    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(20, 20, 20, 20);
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    layout->setStretchFactor(leftLayout, 1);
    layout->setStretchFactor(rightLayout, 1);

    setLayout(layout);
}

void MainWindow::Plot() {

}

void MainWindow::SlotOpenFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QString::fromStdString(GetWorkDirectory()), tr("Excel File(*.xlsx)"));
    if (filePath != "") {
        gInputFile->setText(filePath);
        gDataTable->clearContents();

        using namespace libxl;
        Book * book = xlCreateXMLBook();
        // Book * book = xlCreateBook();  // xlCreateBook() for xls
        if (book) {
            if (book->load(filePath.toStdString().c_str())) {
                Sheet * sheet = book->getSheet(0);
                if (sheet) {
                    gDataTable->setRowCount(sheet->lastRow() - 1);
                    double x_start, y_start, x_end, y_end, flag, x_max = sheet->readNum(1, 0), y_max = sheet->readNum(1, 1);
                    for (int i = 1; i < sheet->lastRow(); ++i) {
                        for (int j = 0; j < sheet->lastCol(); ++j) {
                            if (j == 0) {
                                x_start = sheet->readNum(i, 0);
                                if (x_start > x_max) x_max = x_start;
                            }
                            else if (j == 1) {
                                y_start = sheet->readNum(i, 1);
                                if (y_start > y_max) y_max = y_start;
                            }
                            else if (j == 2) {
                                x_end = sheet->readNum(i, 2);
                                if (x_end > x_max) x_max = x_end;
                            }
                            else if (j == 3) {
                                y_end = sheet->readNum(i, 3);
                                if (y_end > y_max) y_max = y_end;
                            }
                            else if (j == 6) {
                                flag = sheet->readNum(i, 6);
                            }
                            QTableWidgetItem * item = new QTableWidgetItem(QString::number(sheet->readNum(i, j)));
                            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                            gDataTable->setItem(i - 1, j, item);
                        }
                        QCPItemLine * line = new QCPItemLine(gCustomPlot);
                        if (flag == 0.0) {
                            line->setPen(QPen(Qt::red));
                        }
                        else if (flag == 1.0) {
                            line->setPen(QPen(Qt::black));
                        }
                        QCPItemText * textLabel = new QCPItemText(gCustomPlot);
                        textLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);
                        textLabel->position->setCoords(x_start, y_start);
                        textLabel->setText(QString::number(i));
                        textLabel->setFont(QFont(font().family(), 8));
                        textLabel->setPen(QPen(Qt::gray));

                        line->start->setCoords(x_start, y_start);
                        line->end->setCoords(x_end, y_end);

                        line->start->setParentAnchor(textLabel->bottom);
                        line->setHead(QCPLineEnding::esSpikeArrow);
                    }
                    gCustomPlot->xAxis->setTickLabels(true);
                    gCustomPlot->xAxis->setRange(0.0, ceil(x_max));  // TODO
                    gCustomPlot->xAxis->grid()->setPen(QPen(Qt::gray));
                    gCustomPlot->yAxis->setTickLabels(true);
                    gCustomPlot->yAxis->setRange(0.0, ceil(y_max));  // TODO
                    gCustomPlot->yAxis->grid()->setPen(QPen(Qt::gray));
                    gCustomPlot->graph(0)->setName(tr("正面加工路径(红线)\n反面加工路径(黑线)"));  // TODO
                    gCustomPlot->legend->setVisible(true);
                    gCustomPlot->replot();
                    // qApp->processEvents();  // 强制刷新界面
                }
                book->release();
            }
        }
    }
}

void MainWindow::SlotNext() {

}

void MainWindow::SlotQuit() {
    MyMessageBox msgBox;
    msgBox.setText(tr("您要退出系统吗?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int reply = msgBox.exec();
    if (reply == QMessageBox::Yes) {
        QCoreApplication::exit();  // TODO
    }
}

void MainWindow::SlotEditTable() {
    if (gEditFlag) {
        gEditFlag = false;
        gEditButton->setText(tr("禁止编辑"));
        gDataTable->setEditTriggers(QAbstractItemView::CurrentChanged);
    } else {
        gEditFlag = true;
        gEditButton->setText(tr("开启编辑"));
        gDataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void MainWindow::SlotUpdateTable() {
    using namespace libxl;
    Book * book = xlCreateXMLBook();
    // Book * book = xlCreateBook();  // xlCreateBook() for xls
    if (book) {
        if (book->load(gInputFile->text().toStdString().c_str())) {
            Sheet * sheet = book->getSheet(0);
            if (sheet) {
                for (int i = 1; i < sheet->lastRow(); ++i) {
                    for (int j = 0; j < 7; ++j) {
                        sheet->writeNum(i, j, gDataTable->item(i - 1, j)->text().toDouble());
                    }
                }
            }
            book->save((gInputFile->text().toStdString()).c_str());  // NND，需要破解
            book->release();
        }
    }

    MyMessageBox msgBox;
    msgBox.setText(tr("更新表格完毕！"));
    msgBox.setStandardButtons(QMessageBox::Save);
    msgBox.exec();
}
