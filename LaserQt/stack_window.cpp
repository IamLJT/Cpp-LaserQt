#include "stack_window.h"

StackWindow::StackWindow(QWidget * parent) :
    QWidget(parent) {
    CreateMainWindow();
}

StackWindow::~StackWindow() {
    delete gMainWin;
    delete gSecondWin;
    delete gThirdWin;
    delete gFourthWin;
    delete gStackWin;
    delete gPrevButton;
    delete gNextButton;
}

void StackWindow::CreateMainWindow() {
    setMinimumSize(1280, 700);
    setMaximumSize(1280, 700);

    SetWidgets();
}

void StackWindow::SetWidgets() {
    gMainWin = new MainWindow;
    gSecondWin = new SecondWindow;
    gThirdWin = new ThirdWindow;
    gFourthWin = new FourthWindow;

    gStackWin = new QStackedWidget;
    gStackWin->addWidget(gMainWin);
    gStackWin->addWidget(gSecondWin);
    gStackWin->addWidget(gThirdWin);
    gStackWin->addWidget(gFourthWin);
    gStackWinIndex = 0;
    gStackWin->setCurrentIndex(gStackWinIndex);

    /* button layout */
    gPrevButton = new QPushButton;
    gPrevButton->setText( tr("上一步"));
    connect(gPrevButton, SIGNAL(clicked()), this, SLOT(SlotPrev()));
    gPrevButton->setEnabled(false);
    gNextButton = new QPushButton;
    gNextButton->setText(tr("下一步"));
    connect(gNextButton, SIGNAL(clicked()), this, SLOT(SlotNext()));
    QPushButton * quitButton = new QPushButton;
    quitButton->setText(tr("退出"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(SlotQuit()));

    QHBoxLayout * bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->setSpacing(30);
    bottomLayout->addWidget(gPrevButton);
    bottomLayout->addWidget(gNextButton);
    bottomLayout->addWidget(quitButton);

    /* main layout */
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(gStackWin);
    layout->addLayout(bottomLayout);

    setLayout(layout);
}

void StackWindow::SlotPrev() {
    if (gStackWinIndex == 0) {
    }
    else if (gStackWinIndex == 1) {
        gPrevButton->setEnabled(false);
        gStackWinIndex--;
        gStackWin->setCurrentIndex(gStackWinIndex);
    }
    else if (gStackWinIndex == 2) {
        gStackWinIndex--;
        gStackWin->setCurrentIndex(gStackWinIndex);
    }
    else if (gStackWinIndex == 3) {
        gNextButton->setEnabled(true);
        gStackWinIndex -= 2;
        gStackWin->setCurrentIndex(gStackWinIndex);
    }
}

void StackWindow::SlotNext() {
    if (gStackWinIndex == 0) {
        gMainWin->clear();
        gPrevButton->setEnabled(true);
        gStackWinIndex++;
        gStackWin->setCurrentIndex(gStackWinIndex);
    }
    else if (gStackWinIndex == 1) {
        gSecondWin->clear();
        gStackWinIndex++;
        gStackWin->setCurrentIndex(gStackWinIndex);
    }
    else if (gStackWinIndex == 2) {
        gNextButton->setEnabled(false);
        gStackWinIndex++;
        gStackWin->setCurrentIndex(gStackWinIndex);
        gFourthWin->CopyObjectDataFilePath(gThirdWin->CopyObjectDataFilePath());
        gFourthWin->InitWindow();
    }
    else if (gStackWinIndex == 3) {
    }
}

void StackWindow::SlotQuit() {
    MyMessageBox msgBox;
    msgBox.setText(tr("您要退出系统吗?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Yes) {
        gMainWin->close();
        gSecondWin->close();
        gThirdWin->close();
        gFourthWin->close();
        QCoreApplication::exit(0);  // TODO
    }
}

void StackWindow::closeEvent(QCloseEvent * event) {
    MyMessageBox msgBox;
    msgBox.setText(tr("您要退出系统吗?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if (msgBox.exec() == QMessageBox::Yes) {
        event->accept();
    } else {
        event->ignore();
    }
}
