#include "custom_qlabel.h"

void ClickedQLabel::mouseReleaseEvent(QMouseEvent * e) {
    emit clicked();
}
