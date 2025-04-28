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
    ~Bag();

    void Open_bag();

    void Add_pokemon(const QPixmap& IconPixmap, const QString& name);
    void Add_pokeball();
    void Add_potion();
    void Add_ether();
    void Refresh_bag(int id);


private:
    QLabel* bag_image;

    QList<QLabel*> Pokemon_List;

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度

    bool open;
    int pokeball = 0, potion = 0, ether = 0;
    QLabel* pokeballLabel = nullptr;
    QLabel* potionLabel = nullptr;
    QLabel* etherLabel = nullptr;




};

#endif // BAG_H
