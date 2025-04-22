#include "npc.h"

NPC::NPC(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(35, 48); // 整體尺寸

    QPixmap characterPixmap(":/new/prefix1/Dataset/Image/NPC.png");
    this->setPixmap(characterPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}
