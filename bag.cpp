#include "bag.h"

Bag::Bag(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(222, 284);

    QPixmap pixmap(":/new/prefix1/Dataset/Image/bag.png");
    this->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->move(152,84);
}
