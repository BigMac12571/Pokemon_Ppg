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
    PokemonData* GetPokemon_From_List(int id);

    void UpdateHPBar(QLabel* barLabel, int currentHP, int maxHP, QSize size);
    void ResetBattleScene() ;

    void ShowNextDialog();
signals:
    void BattleEnd(bool playerWon);

    void Attack_Dialog(int Who,int MoveID); // attack id =0 , move1 id =1, move2 id =2

    void Items_Dialog(int ItemID); // pokball = 0 , potion =1 , ether = 2

    void DialogFinished();
public slots:
    void Attack_Dialog_slot(int Who,int MoveID);
    void Items_Dialog_slot(int ItemID);

    void Enemy_turn();
    void Player_turn();


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

    bool PlayerTurn;
    bool End;

    void SetupUI();
    void ShowBattleMessage(const QString &msg);


    PokemonData *MyPokemon;
    QLabel* MyHpBarLabel;
    QLabel* MyPokemonImage;
    QLabel* MyPokemonName;
    QLabel* MyLevel;


    PokemonData EnemyPokemon;
    QLabel* EnemyHpBarLabel;
    QLabel* EnemyImage;
    QLabel* EnemyName;
    QLabel* EnemyLevel;


    QWidget* SkillMenu;
    QWidget* bag_area;

    QLabel* Dialog;
    QTimer* Timer;
    int CurrentDialogIndex = 0;
    QList<QStringList> AttackDialogs;
    int Who;
    QList<QStringList> ItemsDialogs;
    int ItemID;
};

#endif // BATTLESCENE_H

