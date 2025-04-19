#include "town.h"

Town::Town(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(525,450);

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/Town.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(0, 0, width(), height()); // 填滿整個視窗
}
void Town::Add_Player_To_Scene(QWidget *player)
{
    player->setParent(this); // 把角色放進這個畫面
    player->setGeometry(200, 300, 64, 64); // 設定顯示位置（你可以再微調）
    player->show(); // 顯示出來
    player->raise();
}
