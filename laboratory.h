#ifndef LABORATORY_H
#define LABORATORY_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include "player.h"
class Laboratory: public QWidget
{
    Q_OBJECT

public:
    explicit Laboratory(QWidget *parent = nullptr);

    void Add_Player_To_Scene(QWidget *player); //按下Press start 後將角色放入視窗
    void SetMainPlayer(Player *p); //
    void UpdateScene() ;//背景移動
    bool CanMoveToDirection(Direction dir); // 為障礙物設計




protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
signals:
    void Exit_Laboratory();
private:

    QLabel *background; // 背景圖片
    Player *mainPlayer = nullptr;
    QPoint Map_Offset; // 地圖的偏移量
    QSet<int> keysPressed; // 長按
    const int Map_Width = 456; // 背景圖片寬度
    const int Map_Height = 455; // 背景圖片高度
    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度
    const int Black_Width = 1840; // 視窗寬度
    const int Black_Height = 1432; // 視窗高度
    //Map_Offset = QPoint(631, 436); //Map_Offset位置
    const int View_Coordinate_x = 678;
    const int View_Coordinate_y = 436;
    const int Player_Center_X = View_Width / 2 - 35 / 2;
    const int Player_Center_Y = View_Height / 2 - 48 / 2;


    QList<QRect> Barriers;
    QRect Exit_Zone;
};

#endif // LABORATORY_H

