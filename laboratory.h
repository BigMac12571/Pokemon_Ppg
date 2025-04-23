#ifndef LABORATORY_H
#define LABORATORY_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
//#include <QDebug>
#include <QKeyEvent>
#include <QPainter>

#include "player.h"
#include "bag.h"
#include "npc.h"
#include "dialog.h"
#include "pokeball.h"
class Laboratory: public QWidget
{
    Q_OBJECT

public:
    explicit Laboratory(QWidget *parent = nullptr);

    void Add_Player_To_Scene(QWidget *player); //按下Press start 後將角色放入視窗
    void Add_NPC_To_Scene(NPC *npc); //按下Press start 後將角色放入視窗
    void Add_Pokeball_To_Scene(int id,Pokeball *pokeballx);
    void Pokeball_get_picked(Pokeball *pokeballx);

    void SetMainPlayer(Player *p); //
    void UpdateScene() ;//背景移動
    bool CanMoveToDirection(Direction dir); // 為障礙物設計
public slots:
    void clearPressedKeys();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
signals:
    void Exit_Laboratory();
    void Open_Dialog_Oak();
    void Show_Pokeballs(int id);

    void Open_Bag_Signal();
private:

    QLabel *background; // 背景圖片
    Player *mainPlayer = nullptr; //
    Bag *bag = nullptr;
    bool OpenBag;
    NPC *ProfessorOak = nullptr ; //存取Oak座標
    //Dialog *dialog = nullptr;
    QList<Pokeball*> pokeball; //存取寶貝球座標



    QPoint Map_Offset; // 地圖的偏移量
    QSet<int> keysPressed; // 長按
    const int Map_Width = 456; // 背景圖片寬度
    const int Map_Height = 455; // 背景圖片高度
    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度
    const int Black_Width = 1840; // 視窗寬度
    const int Black_Height = 1432; // 視窗高度

    const int View_Coordinate_x = 678;
    const int View_Coordinate_y = 436;
    const int Player_Center_X = View_Width / 2 - 35 / 2;
    const int Player_Center_Y = View_Height / 2 - 48 / 2;


    QList<QRect> Barriers;
    QRect Exit_Zone;
    QRect Talk_With_Oak ;


    QList<QRect> Pick_Pokeballs_area; //撿寶貝球的區域
    QList<bool>picked;
};
#endif // TITLESCREEN_H

