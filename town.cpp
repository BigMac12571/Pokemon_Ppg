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

void Town::SetMainPlayer(Player *p) {
    mainPlayer = p;

    mainPlayer->move(525/2, 450/2);
    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();
}

void Town::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();

    // 防止 auto-repeat
    if (keysPressed.contains(key)) return;
    keysPressed.insert(key);

    int step = 5;

    switch (key) {
    case Qt::Key_Up:
        mapOffset.setY(mapOffset.y() - step);
        mainPlayer->setDirection(UP);
        mainPlayer->startWalking();
        break;
    case Qt::Key_Down:
        mapOffset.setY(mapOffset.y() + step);
        mainPlayer->setDirection(DOWN);
        mainPlayer->startWalking();
        break;
    case Qt::Key_Left:
        mapOffset.setX(mapOffset.x() - step);
        mainPlayer->setDirection(LEFT);
        mainPlayer->startWalking();
        break;
    case Qt::Key_Right:
        mapOffset.setX(mapOffset.x() + step);
        mainPlayer->setDirection(RIGHT);
        mainPlayer->startWalking();
        break;
    }

    UpdateScene();
}
void Town::keyReleaseEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();
    keysPressed.remove(key);

    switch (key) {
    case Qt::Key_Up:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(UP);
        }
        break;
    case Qt::Key_Down:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(DOWN);
        }
        break;
    case Qt::Key_Left:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(LEFT);
        }
        break;
    case Qt::Key_Right:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(RIGHT);
        }
        break;
    default:
        break;
    }
}

void Town::UpdateScene()
{
    // 把背景移動相反方向，讓主角看起來有在移動
    background->move(-mapOffset.x(), -mapOffset.y());
}
