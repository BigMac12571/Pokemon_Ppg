#ifndef BAG_H
#define BAG_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QGridLayout>
#include "pokemondata.h"


class Bag:public QWidget
{

public:
    explicit Bag(QWidget *parent = nullptr);
    ~Bag();

    void Open_bag();

    void Refresh_pokemon(QList<QLabel*> &put_in_pokemon); //寶可夢背包管理
    void Add_Pokemon(int id, int form);
    void Remove_Pokemon(int id, int form);
    void ClearBag();


    void Add_pokeball();
    void Add_potion();
    void Add_ether();
    void Refresh_bag(int id);

    int GetPokeball() const {return pokeball ;}
    int GetPotion() const {return potion ;}
    int GetEther() const {return ether ;}

    void UsePokeball();
    void UsePotion();
    void UseEther();

    QList<PokemonData> Pokemon_List;

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap bag_image;
    QWidget* MenuContainer;


    QList<QLabel*> Pokemon_image;
    QList<QLabel*> Pokemon_name;

    QGridLayout *Menu ;
    QList<QList<QWidget*>> pokemonPlaceholders;

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度

    bool open;
    int pokeball = 0, potion = 0, ether = 0;
    QLabel* pokeballLabel = nullptr;
    QLabel* potionLabel = nullptr;
    QLabel* etherLabel = nullptr;
    QLabel* maxPokeballLabel = nullptr;



};

#endif // BAG_H
