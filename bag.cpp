#include "bag.h"

Bag::Bag(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(222, 284);
    bag_image = new QLabel(this);
    QPixmap pixmap(":/new/prefix1/Dataset/Image/bag.png");
    bag_image->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    bag_image->move(0,0);

    open = false;
}

void Bag::Open_bag(){
    if(!open){
        this->show();
        this->raise();
    }
    else {
        this->hide();
       }
    open = !open;
}



//void Bag::Add_pokemon(QWidget* pokemon) {
//    // 繪製簡單 icon
//    QLabel* icon = new QLabel(this);
//    QPixmap pixmap = pokemon->grab();  // 抓出目前畫面來做縮圖
//    icon->setPixmap(pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//    icon->setFixedSize(64, 64);

//    int x = 10 + pokemonList.size() * 70;
//    int y = 10;
//    icon->move(x, y);
//    icon->show();

//    pokemonList.append(pokemon);
//}

