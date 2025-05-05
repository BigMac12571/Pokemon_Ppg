#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
//#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QGraphicsOpacityEffect>

enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};
enum WalkFrame {
    stop = 0,
    walk1 = 1,
    walk2 = 2,
};


class Player: public QLabel
{
    Q_OBJECT

public:

    explicit Player(QWidget *parent = nullptr);

    void setImage(const QString &path);

    void setDirection(Direction dir);
    Direction getDirection();
    void nextWalkFrame(); // 換走路動畫圖
    void startWalking(); //開始走路(用於動畫)
    void stopWalking(); //停止走路(用於動畫)
    bool isWalking();

public slots:
    void updateWalkFrame();  // 0.5 秒換腳




private:

    QLabel *Player_photo; // 角色圖片
    QTimer *walkTimer; // 0.5 秒換腳

    Direction currentDirection = DOWN;
    WalkFrame walkFrame ; // 0: 靜止, 1: 走路圖1, 2: 走路圖2
    WalkFrame lastWalk;
    bool Walking ; //是否在walking
    QVector<QVector<QString>> imagePaths;;


};

#endif // TITLESCREEN_H
