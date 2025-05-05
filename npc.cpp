#include "npc.h"

NPC::NPC(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(35, 48); // 設定角色的大小
    walkFrame = STOP;
    lastWalk = WALK2;
    imagePaths = {
        {":/new/prefix1/Dataset/Image/NPC/Npc_B.png", ":/new/prefix1/Dataset/Image/NPC/Npc_BW1.png", ":/new/prefix1/Dataset/Image/NPC/Npc_BW2.png"},
        {":/new/prefix1/Dataset/Image/NPC/Npc_F.png", ":/new/prefix1/Dataset/Image/NPC/Npc_FW1.png", ":/new/prefix1/Dataset/Image/NPC/Npc_FW2.png"},
        {":/new/prefix1/Dataset/Image/NPC/Npc_L.png", ":/new/prefix1/Dataset/Image/NPC/Npc_LW1.png", ":/new/prefix1/Dataset/Image/NPC/Npc_LW2.png"},
        {":/new/prefix1/Dataset/Image/NPC/Npc_R.png", ":/new/prefix1/Dataset/Image/NPC/Npc_RW1.png", ":/new/prefix1/Dataset/Image/NPC/Npc_RW2.png"}
    };
    Walking = false;

    NPC_photo = new QLabel(this); // 加入子元件
    NPC_photo->setFixedSize(width(), height()); // 設定大小跟 NPC 一樣
    NPC_photo->move(0, 0); // 放在 (0, 0) 位置，才會顯示

    QPixmap characterPixmap(":/new/prefix1/Dataset/Image/NPC/Npc_F.png");
    NPC_photo->setPixmap(characterPixmap.scaled(NPC_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    NPC_photo->show();

    walkTimer = new QTimer(this); //走路換動畫計時器
    walkTimer->setInterval(500);
    walkTimer->start();
    connect(walkTimer, &QTimer::timeout, this, &NPC::updateWalkFrame);


}


void NPC::setImage(const QString &path) { // 放入其中一個圖片
    QPixmap pixmap(path);
    NPC_photo->setPixmap(pixmap.scaled(NPC_photo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void NPC::setDirection(DirectionNPC dir) { //改變方向，
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

void NPC::nextWalkFrame() {
    if (walkFrame == STOP) {   // 從 STOP 進入走路階段，交替使用 WALK1 和 WALK2
        walkFrame = (lastWalk == WALK1) ? WALK2 : WALK1;
        lastWalk = walkFrame;  // 記住上一次是誰
    } else {
        walkFrame = STOP; // 中間插入 STOP 圖片
    }

    setImage(imagePaths[currentDirection][walkFrame]);
}
void NPC::updateWalkFrame() {
    if (Walking) {
        nextWalkFrame();  // 持續走路動畫
    } else {
        walkFrame = STOP;
        setImage(imagePaths[currentDirection][0]); // 停止圖
    }
}


void NPC::startWalking() {
    Walking = true;
}

void NPC::stopWalking() {
    Walking = false;
}

bool NPC::isWalking(){
    return Walking;
}

void NPC::setMapPosition(const QPoint &pos)
{
    mapPosition = pos;
}

QPoint NPC::getMapPosition() const
{
    return mapPosition;
}
