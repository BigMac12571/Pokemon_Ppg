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


enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};



class Player: public QLabel
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = nullptr);

    void setImage(const QString &path);

    void setDirection(Direction dir);
    void nextWalkFrame(); // 換走路動畫圖
    void startWalking();
    void stopWalking();
public slots:
    void updateWalkFrame();  // 0.5 秒換腳

private:

    QLabel *Player_photo; // 角色圖片
    QTimer *walkTimer; // 0.5 秒換腳

    Direction currentDirection = DOWN;
    int walkFrame = 0; // 0: 靜止, 1: 走路圖1, 2: 走路圖2

    QString imagePaths[4][3] = {
        {":/new/prefix1/Dataset/Image/player/player_B.png", ":/new/prefix1/Dataset/Image/player/player_BW1.png", ":/new/prefix1/Dataset/Image/player/player_BW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_F.png", ":/new/prefix1/Dataset/Image/player/player_FW1.png", ":/new/prefix1/Dataset/Image/player/player_FW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_L.png", ":/new/prefix1/Dataset/Image/player/player_LW1.png", ":/new/prefix1/Dataset/Image/player/player_LW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_R.png", ":/new/prefix1/Dataset/Image/player/player_RW1.png", ":/new/prefix1/Dataset/Image/player/player_RW2.png"}
    };


};

#endif // TITLESCREEN_H
