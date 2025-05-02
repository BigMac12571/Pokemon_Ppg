#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QKeyEvent>
#include <QStackedWidget>
#include "titlescreen.h"
#include "laboratory.h"
#include "town.h"
#include "battlescene.h"
#include "grassland.h"
#include "gameover.h"
#include "player.h"
#include "npc.h"
#include "dialog.h"
#include "pokeball.h"
#include "pokemondata.h"
#include "bag.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void switch_windowtitle(int index);

    void Switch_TownToLaboratory();
    void Switch_LaboratoryToTown();

    void Switch_TownToGrassland();
    void Switch_GrasslandToTown();

    void Switch_GrasslandToBattle();
    void Switch_BattelToGrassland(bool win);

    void Oak_Dialog();
    void Sign_Dialog();
    void Grassland_Dialog();

    void Box_Dialog();
    void Show_Pokeballs_Dialog_slot(int id);
    void Close_Dialog();

    void Add_pokeball();
    void Add_potion();
    void Add_ether();
    void Refresh_bag();


    //void Show_Pokeballs_slot(int id);
    void Pickup_Pokeballs_slot(int id);

    void Open_Bag_slot();

    //void GameOver_slot();

private:
    Ui::MainWindow *ui;
    QStackedWidget *Scene_stack;

    TitleScreen *titlescreen;
    Town *town;
    Laboratory *laboratory;
    Grassland *grassland;
    BattleScene *battlescene;
    GameOver *gameover;

    Player *player;
    NPC *npc;
    Dialog *dialog;
    Pokeball *pokeball0;
    Pokeball *pokeball1;
    Pokeball *pokeball2;




    Bag *mybag;
    QList<QLabel*> Pokemon_List;

    QPoint LastMapOffset;
    QPoint PlayPosition;


};
#endif // MAINWINDOW_H
