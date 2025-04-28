#include "box.h"
#include <QPixmap>

Box::Box(QWidget *parent) : QLabel(parent) {
    QPixmap boxPixmap(":/new/prefix1/Dataset/Image/box.png");
    setPixmap(boxPixmap.scaled(33, 33, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setFixedSize(33, 33);
}


