#ifndef GRASSLAND_H
#define GRASSLAND_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QMediaPlayer>
#include <QUrl>
#include <QMediaContent>

#include "player.h"
#include "bag.h"

class Grassland: public QWidget
{
    Q_OBJECT

public:
    explicit Grassland(Bag* mybag,QWidget *parent = nullptr);


    void Add_Player_To_Scene(QWidget *player); //按下Press start 後將角色放入視窗
    void SetMainPlayer(Player *p); //
    void UpdateScene() ;//背景移動
    bool CanMoveToDirection(Direction dir); // 為障礙物設計
    void EncounterBattle();
    void startBattleFlash();

    QPoint GetMapOffset() const { return Map_Offset; }
    void SetLastPosition();
public slots:
    void clearPressedKeys();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
signals:
    void Exit_Grassland();

    void Open_Dialog_Grassland_Sign();

    void Open_Bag_Signal();

    void Refresh_bag();

    void Battle(); // 在草叢裡遇到敵人時觸發


private:

    QLabel *background; // 背景圖片
    Player *mainPlayer = nullptr;
    Bag *bag = nullptr;
    bool OpenBag;
    bool Grass;




    QPoint Map_Offset; // 地圖的偏移量
    QSet<int> keysPressed; // 長按
    const int Map_Width = 1000; // 背景圖片寬度
    const int Map_Height = 1667; // 背景圖片高度
    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度
    const int Player_Center_X = View_Width / 2 - 35 / 2;
    const int Player_Center_Y = View_Height / 2 - 48 / 2;
    QTimer *battleFlashTimer;
    int battleFlashCount;
    int battleFlashInterval;
    QPixmap originalBackgroundPixmap;
    QPixmap flashBackgroundPixmap;
    bool isBattleFlashing = false;



    bool Encountered = false;   // 是否已觸發過戰鬥，避免重複觸發

    QList<QRect> Barriers;
    QList<QRect> Ledges;
    QList<QRect> TallGrasses;// 草叢區域（進入會觸發戰鬥）
    QList<QRect> Talk_With_Sign;



    QRect Exit_Zone;

    QPoint Last_Map_Offset;
    QPoint Last_Player_Pos;

private slots:
    void handleBattleFlashTimeout();
};

#endif // TITLESCREEN_H
