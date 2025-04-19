#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QGraphicsOpacityEffect>

class Player: public QLabel
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);



private:

    QLabel *Player_photo; // 角色圖片

};

#endif // TITLESCREEN_H
