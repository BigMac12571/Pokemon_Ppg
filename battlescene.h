#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include "bulbasaur.h"
#include "squirtle.h"
#include "charmander.h"
#include "bag.h"

class BattleScene : public QWidget
{
    Q_OBJECT

public:
    explicit BattleScene(QWidget *parent = nullptr);

    void StartBattle(Bulbasaur* playerPokemon, Bulbasaur* enemyPokemon);
    void UpdateBattleInfo();
    void UseMove(int moveIndex);
    void UseItem(int itemIndex);
    void RunAway();

signals:
    void BattleEnd(bool playerWon);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *background;
    QLabel *playerLabel;
    QLabel *enemyLabel;
    QLabel *playerInfo;
    QLabel *enemyInfo;
    QLabel *messageBox;

    QPushButton *fightButton;
    QPushButton *bagButton;
    QPushButton *pokemonButton;
    QPushButton *runButton;

    Bulbasaur *playerPokemon;
    Bulbasaur *enemyPokemon;
    Bag *bag;

    bool isPlayerTurn;

    void SetupUI();
    void ShowBattleMessage(const QString &msg);
};

#endif // BATTLESCENE_H

