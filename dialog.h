#ifndef DIALOG_H
#define DIALOG_H


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QStringList>
#include <QRandomGenerator>
#include <QTimer>
#include "pokemondata.h"



class Dialog:public QLabel
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);

    void Oak_Dialog();
    void Sign_Dialog();
    void Grassland_Dialog();

    void Box_Dialog();
    void Show_Pokeballs_Dialog(int id);
    void Show_Pokemon(int id);

//    void Battle_Dialog(PokemonData *pokemon,PokemonData enemy_pokemon,int Attack_or_Item, int Me_Enemy_or_Item_Index , int Attack_or_Item_ID);

    void Reset_Dialog_State();
    //void Setup_Shared_pokeball_ID(int id);




protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void Close_Dialog();

    void Pickup_Pokeballs(int id);
    void Get_pokeball();
    void Get_potion();
    void Get_ether();
private:

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度
    QLabel *Text;



    QStringList Oak_dialog;
    bool Oak_dialog_start;
    QStringList Sign_dialog;
    bool Sign_dialog_start;
    QStringList Grassland_dialog;
    bool Grassland_dialog_start;
//    QList<QList<QStringList>> Battle_dialog;
//    bool Battle_dialog_start;
//    int MoveIndex;
//    PokemonData *MyPokemon = nullptr;
//    PokemonData EnemyPokemon;

    QList<QStringList> Box_dialog;
    bool Box_dialog_start;
    QList<QStringList> Pickup_Pokeballs_dialog;
    bool Pickup_Pokeballs_dialog_start;
    int Shared_pokeball_ID;
    bool Waiting_For_YesNo;
    QLabel *Pokemon;

    int CurrentDialog;







};

#endif // BAG_H
