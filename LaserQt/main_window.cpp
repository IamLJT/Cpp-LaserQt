#include "main_window.h"

MainWindow::MainWindow(QWidget * parent) :
    QWidget(parent) {
    CreateMainWindow();
}

MainWindow::~MainWindow() {
    delete gInputFile;
    delete gDataTable;
    delete gEditButton;
    delete gUpdateButton;
    delete gCustomPlot;
}

void MainWindow::clear() {
    gInputFile->clear();
    gDataTable->clearContents();
    gEditButton->setEnabled(false);
    gUpdateButton->setEnabled(false);
    ClearGraph();
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
    gEditFlag = true;

    /* left-bottom layout */
    gEditButton = new QPushButton(tr("开启编辑"));
    gEditButton->setEnabled(false);
    connect(gEditButton, SIGNAL(clicked()), this, SLOT(SlotEditTable()));
    gUpdateButton = new QPushButton(tr("更新表格"));
    gUpdateButton->setEnabled(false);
    connect(gUpdateButton, SIGNAL(clicked()), this, SLOT(SlotUpdateTable()));

    QHBoxLayout * leftBottomLayout = new QHBoxLayout;
    leftBottomLayout->addStretch();
    leftBottomLayout->addWidget(gEditButton);
    leftBottomLayout->addSpacing(40);
    leftBottomLayout->addWidget(gUpdateButton);

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
    gCustomPlot->graph(0)->setPen(QPen(Qt::red));
    gCustomPlot->addGraph();
    gCustomPlot->graph(1)->setPen(QPen(Qt::black));
    gCustomPlot->plotLayout()->insertRow(0);
    gCustomPlot->plotLayout()->addElement(0, 0, new QCPTextElement(gCustomPlot, tr("加工路径静态图"), QFont(font().family(), 12, QFont::Bold)));
    gCustomPlot->xAxis->setLabel(tr("X-板长方向(m)"));
    gCustomPlot->xAxis->setVisible(true);
    gCustomPlot->xAxis->setTickLabels(false);
    gCustomPlot->yAxis->setLabel(tr("Y-板宽方向(m)"));
    gCustomPlot->yAxis->setVisible(true);
    gCustomPlot->yAxis->setTickLabels(false);
    gCustomPlot->xAxis2->setVisible(true);
    gCustomPlot->xAxis2->setTickLabels(false);
    gCustomPlot->yAxis2->setVisible(true);
    gCustomPlot->yAxis2->setTickLabels(false);

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

void MainWindow::ClearGraph() {
    gCustomPlot->clearItems();
    gCustomPlot->xAxis->setTickLabels(false);
    gCustomPlot->yAxis->setTickLabels(false);
    gCustomPlot->legend->setVisible(false);
    gCustomPlot->replot();
}

void MainWindow::Plot() {
    ClearGraph();

    double x_start, y_start, x_end, y_end, flag;
    double x_max = gDataTable->item(0, 0)->text().toDouble(), y_max = gDataTable->item(0, 1)->text().toDouble();
    double x_min = gDataTable->item(0, 0)->text().toDouble(), y_min = gDataTable->item(0, 1)->text().toDouble();
    for (int i = 0; i < gDataTable->rowCount(); ++i) {
        for (int j = 0; j < 7; ++j) {
            if (j == 0) {
                x_start = gDataTable->item(i, 0)->text().toDouble();
                if (x_start > x_max) x_max = x_start;
                if (x_start < x_min) x_min = x_start;
            }
            else if (j == 1) {
                y_start = gDataTable->item(i, 1)->text().toDouble();
                if (y_start > y_max) y_max = y_start;
                if (y_start < x_min) y_min = y_start;
            }
            else if (j == 2) {
                x_end = gDataTable->item(i, 2)->text().toDouble();
                if (x_end > x_max) x_max = x_end;
                if (x_end < x_min) y_min = x_end;
            }
            else if (j == 3) {
                y_end = gDataTable->item(i, 3)->text().toDouble();
                if (y_end > y_max) y_max = y_end;
                if (y_end < x_min) y_min = y_end;
            }
            else if (j == 6) {
                flag = gDataTable->item(i, 6)->text().toDouble();
            }
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
        textLabel->setText(QString::number(i + 1));
        textLabel->setFont(QFont(font().family(), 8));
        textLabel->setPen(QPen(Qt::gray));

        line->start->setCoords(x_start, y_start);
        line->end->setCoords(x_end, y_end);

        line->start->setParentAnchor(textLabel->bottom);
        line->setHead(QCPLineEnding::esSpikeArrow);
    }
    gCustomPlot->xAxis->setTickLabels(true);
    gCustomPlot->xAxis->setRange(floor(x_min), ceil(x_max));  // TODO
    gCustomPlot->yAxis->setTickLabels(true);
    gCustomPlot->yAxis->setRange(floor(y_min), ceil(y_max));  // TODO
    gCustomPlot->graph(0)->setName(tr("正面加工路径(红线)"));
    gCustomPlot->graph(1)->setName(tr("反面加工路径(黑线)"));
    gCustomPlot->legend->setVisible(true);
    gCustomPlot->legend->setFont(QFont(font().family(), 10, QFont::Bold));
    gCustomPlot->replot();
}

void MainWindow::SlotOpenFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QString::fromStdString(GetWorkDirectory()), tr("Excel File(*.xlsx)"));
    if (filePath != "") {
        gInputFile->setText(filePath);
        gDataTable->clearContents();

#ifdef __linux__
        using namespace libxl;
        Book * book = xlCreateXMLBook();
        // Book * book = xlCreateBook();  // xlCreateBook() for xls
        book->setKey("Michael Hillier", "windows-222329040ec5ec046fb46767a7h1gej6");
        if (book) {
            if (book->load(filePath.toStdString().c_str())) {
                Sheet * sheet = book->getSheet(0);
                if (sheet) {
                    gDataTable->setRowCount(sheet->lastRow() - 1);
                    for (int i = 1; i < sheet->lastRow(); ++i) {
                        for (int j = 0; j < 7; ++j) {
                            QTableWidgetItem * item = new QTableWidgetItem(QString::number(sheet->readNum(i, j)));
                            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                            gDataTable->setItem(i - 1, j, item);
                        }
                    }
                }
                book->release();
                Plot();
                gEditButton->setEnabled(true);
                gUpdateButton->setEnabled(true);
            }
        }
#elif _WIN32
        QXlsx::Document xlsx(filePath);
        gDataTable->setRowCount(xlsx.dimension().lastRow() - 1);
        for (int i = 2; i <= xlsx.dimension().lastRow(); ++i) {
            for (int j = 1; j <= xlsx.dimension().lastColumn(); ++j) {
                if (QXlsx::Cell * cell = xlsx.cellAt(i, j)) {
                    QTableWidgetItem * item = new QTableWidgetItem(cell->value().toString());
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    gDataTable->setItem(i - 2, j - 1, item);
                }
            }
        }
        Plot();
        gEditButton->setEnabled(true);
        gUpdateButton->setEnabled(true);
#endif
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
#ifdef __linux__
    using namespace libxl;
    Book * book = xlCreateXMLBook();
    // Book * book = xlCreateBook();  // xlCreateBook() for xls
    book->setKey("Michael Hillier", "windows-222329040ec5ec046fb46767a7h1gej6");
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
            book->save((gInputFile->text().toStdString()).c_str());
            book->release();
        }
    }
#elif _WIN32
    QXlsx::Document xlsx(gInputFile->text());
    for (int i = 0; i < gDataTable->rowCount(); ++i) {
        for (int j = 0; j < gDataTable->columnCount(); ++j) {
            xlsx.write(i + 2, j + 1, QVariant(gDataTable->item(i, j)->text().toDouble()));
        }
    }
    xlsx.save();
    Plot();

    MyMessageBox msgBox;
    msgBox.setText(tr("更新表格完毕！"));
    msgBox.setStandardButtons(QMessageBox::Save);
    msgBox.exec();
#endif
}
