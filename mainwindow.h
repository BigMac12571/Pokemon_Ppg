#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QKeyEvent>
#include "titlescreen.h"
#include "laboratory.h"
#include "town.h"
#include "battlescene.h"
#include "grassland.h"
#include "player.h"
#include "npc.h"
#include "dialog.h"
#include "pokeball.h"
#include "bulbasaur.h"
#include "charmander.h"
#include "squirtle.h"
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

    void Oak_Dialog();
    void Sign_Dialog();

    void Grassland_Dialog();

    void Show_Pokeballs_Dialog_slot(int id);

    void Close_Dialog();


    void Show_Pokeballs_slot(int id);
    void Pickup_Pokeballs_slot(int id);

    void Open_Bag_slot();
private:
    Ui::MainWindow *ui;
    QStackedWidget *Scene_stack;

    TitleScreen *titlescreen;
    Town *town;
    Laboratory *laboratory;
    Grassland *grassland;
    BattleScene *battlescene;

    Player *player;
    NPC *npc;
    Dialog *dialog;
    Pokeball *pokeball0;
    Pokeball *pokeball1;
    Pokeball *pokeball2;


    Bulbasaur *bulbasaur;
    Squirtle *squirtle;
    Charmander *charmander;

    Bag *bag;

};
#endif // MAINWINDOW_H
