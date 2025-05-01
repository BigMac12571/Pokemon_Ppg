#include "gameover.h"

GameOver::GameOver(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(525,450);

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/game_over.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(0, 0, width(), height()); // 填滿整個視窗

}
