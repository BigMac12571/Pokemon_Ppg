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
    player->setParent(this);
    player->setGeometry(200, 300, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方
}

void Town::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int step = 10;
    QPoint currentPos = mainPlayer->pos();

    switch (event->key()) {
    case Qt::Key_Up:
        mainPlayer->move(currentPos.x(), currentPos.y() - step);
        break;
    case Qt::Key_Down:
        mainPlayer->move(currentPos.x(), currentPos.y() + step);
        break;
    case Qt::Key_Left:
        mainPlayer->move(currentPos.x() - step, currentPos.y());
        break;
    case Qt::Key_Right:
        mainPlayer->move(currentPos.x() + step, currentPos.y());
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}
