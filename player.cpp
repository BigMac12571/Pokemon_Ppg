#include "player.h"

Player::Player(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(35, 48); // 設定角色的大小

    Player_photo = new QLabel(this); // 加入子元件
    Player_photo->setFixedSize(35, 48); // 設定大小跟 Player 一樣
    Player_photo->move(0, 0); // 放在 (0, 0) 位置，才會顯示

    QPixmap characterPixmap(":/new/prefix1/Dataset/Image/player/player_F.png");
    Player_photo->setPixmap(characterPixmap.scaled(Player_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Player_photo->show();



}
void Player::setImage(const QString &path) {
    QPixmap pixmap(path);
    Player_photo->setPixmap(pixmap.scaled(Player_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
