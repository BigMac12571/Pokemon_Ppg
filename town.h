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

protected:
    void keyPressEvent(QKeyEvent *event) override;


private:

    QLabel *background; // 背景圖片
    Player *mainPlayer = nullptr;
};

#endif // TITLESCREEN_H
