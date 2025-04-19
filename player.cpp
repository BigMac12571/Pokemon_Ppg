#include "player.h"

Player::Player(QWidget *parent) : QLabel(parent)
{


    Player_photo = new QLabel(this);
    QPixmap characterPixmap(":/new/prefix1/Dataset/Image/player/player_F.png");
    Player_photo->setPixmap(characterPixmap.scaled(100, 100, Qt::KeepAspectRatio));
    Player_photo->setGeometry(525/2, 225, 35, 48);





}
