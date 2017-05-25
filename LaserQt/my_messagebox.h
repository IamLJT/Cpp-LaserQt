#ifndef MY_MESSAGEBOX_H
#define MY_MESSAGEBOX_H

#include <QLabel>
#include <QMessageBox>

class MyMessageBox : public QMessageBox {
protected:
    void showEvent(QShowEvent * event) {
        QMessageBox::showEvent(event);
//        QWidget * textField = findChild<QWidget *>("qt_msgbox_label");
//        if (textField  != NULL) {
//            textField ->setMinimumSize(300, 60);
//        }
//        QList<QLabel *> textFieldLabels = findChildren<QLabel *>();
//        for(int i = 0; i < textFieldLabels.size(); ++i) {
//            textFieldLabels.at(i)->setAlignment(Qt::AlignCenter);
//        }
    }
};

#endif // MY_MESSAGEBOX_H
