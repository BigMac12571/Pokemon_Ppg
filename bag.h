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




class Bag:public QLabel
{

public:
    explicit Bag(QWidget *parent = nullptr);

private:

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度








};

#endif // BAG_H
