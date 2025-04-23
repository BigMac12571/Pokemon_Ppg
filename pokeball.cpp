#include "pokeball.h"

Pokeball::Pokeball(int id, QWidget *parent)
    : QLabel(parent), ID(id)
{

    QPixmap PokeballPixmap(":/new/prefix1/Dataset/Image/ball.png");
    this->setFixedSize(35, 35);
    this->setPixmap(PokeballPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


