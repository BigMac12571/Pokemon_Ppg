#include "bag.h"

Bag::Bag(QWidget *parent) : QLabel(parent)
{


    this->setFixedSize(148, 189);

       // 初始化 Bag_photo
    Bag_photo = new QLabel(this); // 記得設 parent 為 this
    Bag_photo->setFixedSize(this->size()); // 讓圖片區域與 Bag 大小一致

    QPixmap characterPixmap(":/new/prefix1/Dataset/Image/bag.png");
    Bag_photo->setPixmap(characterPixmap.scaled(Bag_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Bag_photo->move(0, 0);
    Bag_photo->show();







}
