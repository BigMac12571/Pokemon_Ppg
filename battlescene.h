#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include "pokemondata.h"
#include "bag.h"

class BattleScene : public QWidget
{
    Q_OBJECT

public:
    explicit BattleScene(Bag *mybag,QWidget *parent = nullptr);

    void StartBattle();
    void UpdateBattleInfo();
    void RebuildAllButtons();
    void UseMove(int moveIndex);
    void UseItem(int itemIndex);

    PokemonData GenerateRandomEnemy();
    PokemonData* GetPokemon_From_List(int id);

    void UpdateHPBar(QLabel* barLabel, int currentHP, int maxHP, QSize size);

    void ResetBattleScene() ;

    void ShowNextDialog();

    void Pokeball_Animation_Start();
    void CapturePokemon();
    void Dead_And_SwitchToAnotherPokemon();
signals:
    void BattleEnd(bool playerWon);

    void Attack_Dialog(int Who,int MoveID, bool special = false); // attack id =0 , move1 id =1, move2 id =2
    void Items_Dialog(int ItemID); // pokball = 0 , potion =1 , ether = 2
    //void Pokemon_Switch_Dialog(int PokemonID); // pokball = 0 , potion =1 , ether = 2

    void DialogFinished();

    void GameOver();
public slots:
    void Attack_Dialog_slot(int Who,int MoveID, bool special = false);
    void Items_Dialog_slot(int ItemID);
    void Pokemon_Switch_Dialog_slot(int Seletion);
    void Pokemon_Dead_Dialog_slot();

    void Enemy_turn();
    void Player_turn();


protected:

//    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    QLabel *background;


    QToolButton *fightButton;
    QToolButton *bagButton;
    QToolButton *pokemonButton;
    QToolButton *runButton;

    int currentSkillIndex = 0;
    QList<QList<QToolButton*>> Buttons;//0 => Move ,1=> MovePP,2 => SpecialATK, 3=>Item, => Pokemon



    Bag *bag;

    bool PlayerTurn;
    bool End;

    void SetupUI();



    PokemonData *MyPokemon;
    QLabel* MyHpBarLabel;
    QLabel* MyHp;
    QLabel* MyPokemonImage;
    QPixmap MyImage;
    QLabel* MyPokemonName;
    QLabel* MyLevel;


    PokemonData EnemyPokemon;
    QLabel* EnemyHpBarLabel;
    QLabel* EnemyImage;
    QLabel* EnemyName;
    QLabel* EnemyLevel;

    QWidget* MainMenu;
    QWidget* SkillMenu;
    QWidget* ItemBagMenu;
    QWidget* Restore_PP_Menu;
    QWidget* PokemonMenu;
    QWidget* SpecialMenu;
    QLabel* SkillInfo_SkillMenu;
    QLabel* SkillInfo_PPMenu;
    QList<QLabel*> Type_SkillMenu;
    QList<QLabel*> Type_PPMenu;

    QWidget* Pokeball_Animation;

    QLabel* Dialog;
    QTimer* Timer;
    int CurrentDialogIndex = 0;
    QList<QStringList> AttackDialogs;
    int Who;
    QList<QStringList> ItemsDialogs;
    int ItemID;
    int RestoreMoveID;
    bool Capture;
    QList<QStringList> Pokemon_Switch_Dialogs;
    QList<QStringList> Pokemon_Dead_Dialogs;
};

#endif // BATTLESCENE_H

