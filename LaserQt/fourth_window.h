#ifndef FORTHWINDOW_H
#define FORTHWINDOW_H

#include <Q3DScatter>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "auxiliary_function.h"
#include "custom_qlabel.h"
#include "my_messagebox.h"
#include "qcustomplot.h"

class QWidget;

class FourthWindow : public QWidget {
    Q_OBJECT

public:
    explicit FourthWindow(QWidget *parent = 0);
    ~FourthWindow();

private:
    QCustomPlot * gErrorCanvas_1;
    QCustomPlot * gErrorCanvas_2;
    QCustomPlot * gErrorCanvas_3;
    QCustomPlot * gErrorCanvas_4;
    QCustomPlot * gErrorCanvas_5;
    QCustomPlot * gErrorCanvas_6;
    ClickedQLabel * gLeftArrowLabel;
    ClickedQLabel * gRightArrowLabel;
    QStackedWidget * gStackWin;
    int gStackWinIndex;
    QLineEdit * gXStart;
    QLineEdit * gYStart;
    QLineEdit * gXEnd;
    QLineEdit * gYEnd;
    QPushButton * gOKButton;
    QtDataVisualization::Q3DScatter * gPointCloudDataGraph;

    void CreateMainWindow();
    void SetWidgets();

private slots:
    void SlotLeftArrowClicked();
    void SlotRightArrowClicked();
    void SlotOK();
};

#endif // FORTHWINDOW_H
