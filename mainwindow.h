#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
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

private:
    Ui::MainWindow *ui;
    QStackedWidget *Scene_stack;
    QStackedWidget *Character_stack;

    TitleScreen *titlescreen;
    Town *town;
    Laboratory *laboratary;
    GrassLand *grassland;
    BattleScene *battlescene;

    Player *player[12];


};
#endif // MAINWINDOW_H
