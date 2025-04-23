#include "box.h"
#include <QPixmap>

Box::Box(QWidget *parent) : QLabel(parent), picked(false) {
    QPixmap boxPixmap(":/new/prefix1/Dataset/Image/box.png");
    setPixmap(boxPixmap.scaled(33, 33, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setFixedSize(33, 33);
}

bool Box::isPicked() const {
    return picked;
}

void Box::pick() {
    if (!picked) {
        picked = true;
        this->hide();  // 撿起時隱藏箱子
    }
}
