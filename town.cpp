#include "town.h"

Town::Town(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(1000,1000);

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/Town.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-250, -250, width(), height()); // 填滿整個視窗



    setFocusPolicy(Qt::StrongFocus);
}
void Town::Add_Player_To_Scene(QWidget *player)
{
    player->setParent(this);
    player->setGeometry(525/2, 450/2, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方
}

void Town::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

        int step = 10;

        switch (event->key()) {
        case Qt::Key_Up:
            mapOffset.setY(mapOffset.y() - step);
            mainPlayer->setDirection(UP);
            mainPlayer->nextWalkFrame();
            break;
        case Qt::Key_Down:
            mapOffset.setY(mapOffset.y() + step);
            mainPlayer->setDirection(DOWN);
            mainPlayer->nextWalkFrame();
            break;
        case Qt::Key_Left:
            mapOffset.setX(mapOffset.x() - step);
            mainPlayer->setDirection(LEFT);
            mainPlayer->nextWalkFrame();
            break;
        case Qt::Key_Right:
            mapOffset.setX(mapOffset.x() + step);
            mainPlayer->setDirection(RIGHT);
            mainPlayer->nextWalkFrame();
            break;
        }

        UpdateScene();
}


void Town::SetMainPlayer(Player *p) {
    mainPlayer = p;

    mainPlayer->move(525/2, 450/2);
    mainPlayer->raise();
    mainPlayer->setFocus();
}

void Town::UpdateScene()
{
    // 把背景移動相反方向，讓主角看起來有在移動
    background->move(-mapOffset.x(), -mapOffset.y());
}
