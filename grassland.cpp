#include "grassland.h"

GrassLand::GrassLand(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(Map_Width,Map_Height);

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/GrassLand.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-Player_Center_X, -Player_Center_X, width(), height()); // 填滿整個視窗
    background->lower(); //背景在最下

    Map_Offset = QPoint(View_Coordinate_x-(View_Width-Map_Width)/2, View_Coordinate_y-Player_Center_Y+Map_Height-100); //Map_Offset位置
}
