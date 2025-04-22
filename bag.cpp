#include "bag.h"

Bag::Bag(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(148, 189);

    QPixmap pixmap(":/new/prefix1/Dataset/Image/bag.png");
    this->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
