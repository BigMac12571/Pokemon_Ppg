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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void switch_windowtitle(int index);

    void Switch_TownToLaboratory();
    void Switch_LaboratoryToTown();



    void Switch_TownToGrassland();
    void Switch_GrasslandToTown();



private:
    Ui::MainWindow *ui;
    QStackedWidget *Scene_stack;
    QStackedWidget *Character_stack;

    TitleScreen *titlescreen;
    Town *town;
    Laboratory *laboratary;
    Grassland *grassland;
    BattleScene *battlescene;

    Player *player;


};
#endif // MAINWINDOW_H
