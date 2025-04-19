#ifndef TOWN_H
#define TOWN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include "player.h"

class Town: public QWidget
{
    Q_OBJECT

public:
    explicit Town(QWidget *parent = nullptr);
    void Add_Player_To_Scene(QWidget *player);
    void SetMainPlayer(Player *p);
    void UpdateScene() ;//背景移動

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:

    QLabel *background; // 背景圖片
    Player *mainPlayer = nullptr;
    QPoint mapOffset = QPoint(250, 250); // 地圖的偏移量
    QSet<int> keysPressed; // 長按
};

#endif // TITLESCREEN_H
