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
    void RebuildAllButtons();
    void UseMove(int moveIndex);
    void UseItem(int itemIndex);
    void RunAway();

    PokemonData GenerateRandomEnemy();
    PokemonData* GetPokemon_From_List(int id);

    void UpdateHPBar(QLabel* barLabel, int currentHP, int maxHP, QSize size);

    void ResetBattleScene() ;

    void ShowNextDialog();

    void Pokeball_Animation_Start();
    void CapturePokemon();
signals:
    void BattleEnd(bool playerWon);

    void Attack_Dialog(int Who,int MoveID); // attack id =0 , move1 id =1, move2 id =2
    void Items_Dialog(int ItemID); // pokball = 0 , potion =1 , ether = 2
    void Pokemon_Switch_Dialog(int PokemonID); // pokball = 0 , potion =1 , ether = 2

    void DialogFinished();
public slots:
    void Attack_Dialog_slot(int Who,int MoveID);
    void Items_Dialog_slot(int ItemID);
    void Pokemon_Switch_Dialog_slot(int PokemonID);

    void Enemy_turn();
    void Player_turn();


protected:
    void paintEvent(QPaintEvent *event) override;
//    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
private:
    QLabel *background;
    QLabel *playerLabel;
    QLabel *enemyLabel;
    QLabel *playerInfo;
    QLabel *enemyInfo;
    QLabel *messageBox;

    QToolButton *fightButton;
    QToolButton *bagButton;
    QToolButton *pokemonButton;
    QToolButton *runButton;

    int currentSkillIndex = 0;
    QList<QList<QToolButton*>> Buttons;//0 => Move ,1=> MovePP, 2=>Item, 3=> Pokemon
    QToolButton* backButton_skill;
    QToolButton* Move0_Button;
    QToolButton* Move1_Button;
    QToolButton* Move2_Button;
    QToolButton* Move3_Button;

    QToolButton* Pokeball_Button ;
    QToolButton* Potion_Button ;
    QToolButton* Ether_Button ;
    QToolButton* Bag_BackButton ;

    QToolButton* backButton_Pokemon;
    QToolButton* Pokemon0 ;
    QToolButton* Pokemon1 ;
    QToolButton* Pokemon2 ;
    QToolButton* Pokemon3 ;

    QToolButton* backButton_PP;
    QToolButton* Move0_Button_PP;
    QToolButton* Move1_Button_PP;
    QToolButton* Move2_Button_PP;
    QToolButton* Move3_Button_PP;

    Bulbasaur *playerPokemon;
    Bulbasaur *enemyPokemon;
    Bag *bag;

    bool PlayerTurn;
    bool End;

    void SetupUI();
    void ShowBattleMessage(const QString &msg);


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
    QLabel* SkillInfo_SkillMenu;
    QLabel* SkillInfo_PPMenu;

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
};

#endif // BATTLESCENE_H

