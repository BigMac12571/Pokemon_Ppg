#ifndef TOWN_H
#define TOWN_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

class Town: public QWidget
{
    Q_OBJECT

public:
    explicit Town(QWidget *parent = nullptr);
    void Add_Player_To_Scene(QWidget *player);


private:

    QLabel *background; // 背景圖片

};

#endif // TITLESCREEN_H
