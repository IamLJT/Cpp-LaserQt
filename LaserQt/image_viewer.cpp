#include "image_viewer.h"
#include <QDebug>

ImageViewer::ImageViewer(QWidget * parent, const QStringList &path) :
    QWidget(parent), gImagePath(path) {
    gCurrentImage = 0;
    gTotalImages = gImagePath.size();
    CreateMainWindow();
}

ImageViewer::~ImageViewer() {

}

void ImageViewer::CreateMainWindow() {
    setWindowTitle("Image Viewer");
    setMinimumSize(960, 540);
    setMaximumSize(960, 540);

    SetWidgets();
}

void ImageViewer::SetWidgets() {
    /* left layout */
    gViewer = new QLabel;
    gViewer->setPixmap(QPixmap(gImagePath.at(gCurrentImage)));
    gLeftArrowLabel = new ClickedQLabel();
    gLeftArrowLabel->setPixmap(QPixmap::fromImage(QImage(":/img/ui/arrow_left.png")));
    gLeftArrowLabel->setAlignment(Qt::AlignCenter);
    connect(gLeftArrowLabel, SIGNAL(clicked()), this, SLOT(SlotLeftArrowClicked()));
    gRightArrowLabel = new ClickedQLabel();
    gRightArrowLabel->setPixmap(QPixmap::fromImage(QImage(":/img/ui/arrow_right.png")));
    gRightArrowLabel->setAlignment(Qt::AlignCenter);
    connect(gRightArrowLabel, SIGNAL(clicked()), this, SLOT(SlotRightArrowClicked()));

    QHBoxLayout * leftLayout = new QHBoxLayout;
    leftLayout->addStretch();
    leftLayout->setSpacing(10);
    leftLayout->addWidget(gLeftArrowLabel);
    leftLayout->addWidget(gViewer);
    leftLayout->addWidget(gRightArrowLabel);

    /* right layout */
    gImagePathList = new QListWidget;
    for (int i = 0; i < gImagePath.size(); ++i) {
        QListWidgetItem * newItem = new QListWidgetItem;
        newItem->setText(gImagePath.at(i));
        gImagePathList->insertItem(i, newItem);
    }
    gImagePathList->setCurrentRow(gCurrentImage);
    connect(gImagePathList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SlotListItemClicked()));

    QHBoxLayout * rightLayout = new QHBoxLayout;
    rightLayout->setContentsMargins(0, 15, 0, 15);
    rightLayout->addWidget(gImagePathList);

    /* main layout */
    QHBoxLayout * layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    setLayout(layout);
}

void ImageViewer::SlotLeftArrowClicked() {
    if (gCurrentImage == 0) {
        gCurrentImage = gTotalImages - 1;
    } else {
        gCurrentImage--;
    }

    gViewer->setPixmap(QPixmap(gImagePath.at(gCurrentImage)));
    gImagePathList->setCurrentRow(gCurrentImage);
}

void ImageViewer::SlotRightArrowClicked() {
    if (gCurrentImage == gTotalImages - 1) {
        gCurrentImage = 0;
    } else {
        gCurrentImage++;
    }

    gViewer->setPixmap(QPixmap(gImagePath.at(gCurrentImage)));
    gImagePathList->setCurrentRow(gCurrentImage);
}

void ImageViewer::SlotListItemClicked() {
    gCurrentImage = gImagePathList->currentRow();
    gViewer->setPixmap(QPixmap(gImagePath.at(gCurrentImage)));
}
