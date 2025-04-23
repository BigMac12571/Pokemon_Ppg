#ifndef BAG_H
#define BAG_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
//#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include "bulbasaur.h"
#include "squirtle.h"
#include "charmander.h"



class Bag:public QWidget
{

public:
    explicit Bag(QWidget *parent = nullptr);

    void Open_bag();

//    void Add_pokemon(QWidget* pokemon);



private:
    QLabel* bag_image;
    QLabel* bulbasaur_image;
    QLabel* squirtle_image;
    QLabel* charmander_image;

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度

    bool open;





};

#endif // BAG_H
