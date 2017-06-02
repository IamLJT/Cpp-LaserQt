#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "auxiliary_function.h"
#include "custom_qlabel.h"

class ImageViewer : public QWidget {
    Q_OBJECT

public:
    explicit ImageViewer(QWidget * parent, const QStringList &path);
    ~ImageViewer();

private:
    QStringList gImagePath;
    QLabel * gViewer;
    ClickedQLabel * gLeftArrowLabel;
    ClickedQLabel * gRightArrowLabel;
    QListWidget * gImagePathList;
    int32_t gCurrentImage;
    int32_t gTotalImages;

    void CreateMainWindow();
    void SetWidgets();

private slots:
    void SlotLeftArrowClicked();
    void SlotRightArrowClicked();
    void SlotListItemClicked();
};

#endif // IMAGE_VIEWER_H
