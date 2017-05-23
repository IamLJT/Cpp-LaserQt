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
    leftBottomLayout->setSpacing(40);
    leftBottomLayout->addWidget(gEditButton);
    leftBottomLayout->addWidget(updateButton);
    leftBottomLayout->addWidget(nextButton);
    leftBottomLayout->addWidget(quitButton);

    /* left layout */
    QVBoxLayout * leftLayout = new QVBoxLayout;
    leftLayout->addLayout(leftTopLayout);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(gDataTable);
    leftLayout->addSpacing(40);
    leftLayout->addLayout(leftBottomLayout);

    /* right-bottom layout */
    QHBoxLayout * rightBottomLayout = new QHBoxLayout;

    /* right layout */
    gCustomPlot = new QCustomPlot;
    gCustomPlot->addGraph();

    // gCustomPlot->plotLayout()->insertRow(0);
    // gCustomPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(gCustomPlot, tr("加工路径静态图")));
    gCustomPlot->xAxis->setLabel(tr("X-板长方向(m)"));
    gCustomPlot->xAxis->setVisible(true);
    gCustomPlot->xAxis->setTickLabels(false);
    gCustomPlot->yAxis->setLabel(tr("Y-板宽方向(m)"));
    gCustomPlot->yAxis->setVisible(true);
    gCustomPlot->yAxis->setTickLabels(false);

    QVBoxLayout * rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0, 52, 0, 72);
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
                    gDataTable->setRowCount(sheet->lastRow());
                    for (int i = 0; i < sheet->lastRow(); ++i) {
                        for (int j = 0; j < 7; ++j) {
                            QTableWidgetItem * item = new QTableWidgetItem(QString::number(sheet->readNum(i, j)));
                            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                            gDataTable->setItem(i, j, item);
                        }
                    }
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
                for (int i = 0; i < sheet->lastRow(); ++i) {
                    for (int j = 0; j < 7; ++j) {
                        sheet->writeNum(i, j, gDataTable->item(i, j)->text().toDouble());
                    }
                }
            }
            // book->save(book->load(gInputFile->text().toStdString().c_str()));  // TODO
            book->release();
        }
    }

    MyMessageBox msgBox;
    msgBox.setText(tr("更新表格完毕！"));
    msgBox.setStandardButtons(QMessageBox::Save);
    msgBox.exec();
}
