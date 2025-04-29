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
#include "pokemondata.h"
#include "bag.h"

class BattleScene : public QWidget
{
    Q_OBJECT

public:
    explicit BattleScene(Bag *mybag,QWidget *parent = nullptr);

    void StartBattle();
    void UpdateBattleInfo();
    void UseMove(int moveIndex);
    void UseItem(int itemIndex);
    void RunAway();

    PokemonData GenerateRandomEnemy();
    PokemonData GetPokemon_From_List(int id);

    void UpdateHPBar(QLabel* barLabel, int currentHP, int maxHP, QSize size);
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


    PokemonData myPokemon;
    PokemonData EnemyPokemon;
    QLabel* myHpBarLabel;
    QLabel* enemyHpBarLabel;

    QWidget* SkillMenu;
};

#endif // BATTLESCENE_H

