#ifndef BAG_H
#define BAG_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>




class Bag:public QWidget
{

public:
    explicit Bag(QWidget *parent = nullptr);

    void Open_bag();

    void Add_pokemon(const QPixmap& IconPixmap, const QString& name);



private:
    QLabel* bag_image;

    QList<QLabel*> Pokemon_List;

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度

    bool open;





};

#endif // BAG_H
