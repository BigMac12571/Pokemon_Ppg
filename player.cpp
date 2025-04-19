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


    walkTimer = new QTimer(this);
    walkTimer->setInterval(500);
    connect(walkTimer, &QTimer::timeout, this, &Player::updateWalkFrame);



}


void Player::setImage(const QString &path) {
    QPixmap pixmap(path);
    Player_photo->setPixmap(pixmap.scaled(Player_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Player::setDirection(Direction dir) {
    currentDirection = dir;
    walkFrame = 0; // 回到靜止圖
    setImage(imagePaths[dir][0]);
}

void Player::nextWalkFrame() {
    walkFrame = (walkFrame == 1) ? 2 : 1; // 在動畫1與2間切換
    setImage(imagePaths[currentDirection][walkFrame]);
}
void Player::updateWalkFrame() {
    nextWalkFrame(); // 呼叫你原本的切換方法
}
void Player::startWalking() {
    if (!walkTimer->isActive()) {
        walkTimer->start();
    }
}

void Player::stopWalking() {
    walkTimer->stop();
    walkFrame = 0; // 回到靜止
    setImage(imagePaths[currentDirection][0]);
}
