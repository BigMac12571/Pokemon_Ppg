#include "box.h"

Box::Box(QWidget *parent) : QLabel(parent) {
    QPixmap boxPixmap(":/new/prefix1/Dataset/Image/box.png"); // 假設你有 box 的圖片
    setPixmap(boxPixmap.scaled(33, 33, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setFixedSize(33, 33);
}
