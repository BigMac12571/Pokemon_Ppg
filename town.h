#ifndef TOWN_H
#define TOWN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include "player.h"
#include "bag.h"



class Town: public QWidget
{
    Q_OBJECT

public:
    explicit Town(QWidget *parent = nullptr);

    void Add_Player_To_Scene(QWidget *player); //按下Press start 後將角色放入視窗

    void SetMainPlayer(Player *p); //
    void SetMainPlayer_GrasslandToTown(Player *p);







    void UpdateScene() ;//背景移動
    bool CanMoveToDirection(Direction dir); // 為障礙物設計




protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
signals:
    void Enter_Laboratory();
    void Enter_Grassland();
private:


    Player *mainPlayer = nullptr;
    Bag *bag = nullptr;
    bool OpenBag;

    QLabel *background; // 背景圖片
    QPoint Map_Offset; // 地圖的偏移量
    QSet<int> keysPressed; // 長按
    const int Map_Width = 1000; // 背景圖片寬度（例如 Town.png 寬度）
    const int Map_Height = 1000; // 背景圖片高度
    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度
    const int Player_Center_X = View_Width / 2 - 35 / 2;
    const int Player_Center_Y = View_Height / 2 - 48 / 2;



    QList<QRect> Barriers;
    QRect Enter_Laboratory_Trigger; //進入實驗室
    QRect Enter_Grassland_Trigger; //進入草地區

};

#endif // TOWN_H

