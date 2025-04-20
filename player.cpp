#include "player.h"

Player::Player(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(35, 48); // 設定角色的大小
    walkFrame = stop;
    lastWalk = walk2;
    imagePaths = {
        {":/new/prefix1/Dataset/Image/player/player_B.png", ":/new/prefix1/Dataset/Image/player/player_BW1.png", ":/new/prefix1/Dataset/Image/player/player_BW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_F.png", ":/new/prefix1/Dataset/Image/player/player_FW1.png", ":/new/prefix1/Dataset/Image/player/player_FW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_L.png", ":/new/prefix1/Dataset/Image/player/player_LW1.png", ":/new/prefix1/Dataset/Image/player/player_LW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_R.png", ":/new/prefix1/Dataset/Image/player/player_RW1.png", ":/new/prefix1/Dataset/Image/player/player_RW2.png"}
    };
    Walking = false;

    Player_photo = new QLabel(this); // 加入子元件
    Player_photo->setFixedSize(35, 48); // 設定大小跟 Player 一樣
    Player_photo->move(0, 0); // 放在 (0, 0) 位置，才會顯示

    QPixmap characterPixmap(":/new/prefix1/Dataset/Image/player/player_F.png");
    Player_photo->setPixmap(characterPixmap.scaled(Player_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Player_photo->show();

    walkTimer = new QTimer(this); //走路換動畫計時器
    walkTimer->setInterval(200);
    walkTimer->start();
    connect(walkTimer, &QTimer::timeout, this, &Player::updateWalkFrame);


}


void Player::setImage(const QString &path) { // 放入其中一個圖片
    QPixmap pixmap(path);
    Player_photo->setPixmap(pixmap.scaled(Player_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Player::setDirection(Direction dir) { //改變方向，
    if (currentDirection != dir) {
        currentDirection = dir;
        if (Walking) {
            // 如果正在走路，馬上跳下一個動畫幀（會根據新方向）
            nextWalkFrame();
        } else {
            // 沒在走路，就放靜止圖
            setImage(imagePaths[currentDirection][0]);
        }
    }
}

void Player::nextWalkFrame() {
    if (walkFrame == stop) {   // 從 stop 進入走路階段，交替使用 walk1 和 walk2
        walkFrame = (lastWalk == walk1) ? walk2 : walk1;
        lastWalk = walkFrame;  // 記住上一次是誰
    } else {
        walkFrame = stop; // 中間插入 stop 圖片
    }

    setImage(imagePaths[currentDirection][walkFrame]);
}
void Player::updateWalkFrame() {
    if (Walking) {
        nextWalkFrame();  // 持續走路動畫
    } else {
        walkFrame = stop;
        setImage(imagePaths[currentDirection][0]); // 停止圖
    }
}


void Player::startWalking() {
    Walking = true;
}

void Player::stopWalking() {
    Walking = false;
}
