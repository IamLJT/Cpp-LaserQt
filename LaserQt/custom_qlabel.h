#ifndef CUSTOM_QLABEL_H
#define CUSTOM_QLABEL_H

#include <QLabel>

class ClickedQLabel : public QLabel
{
    Q_OBJECT

protected:
    void mouseReleaseEvent(QMouseEvent * e);

signals:
    void clicked();
};

#endif // CUSTOM_QLABEL_H
