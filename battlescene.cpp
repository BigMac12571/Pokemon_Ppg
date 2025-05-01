#include "battlescene.h"
#include <QDebug>
#include <QTimer>

BattleScene::BattleScene(Bag *myBag, QWidget *parent)
    : QWidget(parent), playerPokemon(nullptr), enemyPokemon(nullptr), bag(myBag) , PlayerTurn(true),MyPokemon(nullptr),EnemyPokemon(0,1)
{
    if (!bag) {
            qDebug() << "❌ Bag 是空指標！";
        } else {
            qDebug() << "✅ Bag 有效，大小：" << bag->Pokemon_List.size();
        }

    SetupUI();
}

void BattleScene::SetupUI() {
    background = new QLabel(this);
    background->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/battle/battle_scene.png"));
    background->setGeometry(0, 0, 525, 450);
    MainMenu = new QWidget(this);

    // 建立四個指令選單按鈕
    fightButton = new QToolButton(MainMenu);
    fightButton->setFocusPolicy(Qt::NoFocus);
    fightButton->setText("FIGHT");
    fightButton->setGeometry(290, 345, 100, 30);

    bagButton = new QToolButton(MainMenu);
    bagButton->setFocusPolicy(Qt::NoFocus);
    bagButton->setText("BAG");
    bagButton->setGeometry(400, 345, 100, 30);

    pokemonButton = new QToolButton(MainMenu);
    pokemonButton->setFocusPolicy(Qt::NoFocus);
    pokemonButton->setText("POKEMON");
    pokemonButton->setGeometry(290, 385, 100, 30);

    runButton = new QToolButton(MainMenu);
    runButton->setFocusPolicy(Qt::NoFocus);
    runButton->setText("RUN");
    runButton->setGeometry(400, 385, 100, 30);



    // 位置模擬 GBA 排列





    connect(runButton, &QPushButton::clicked, this, &BattleScene::RunAway);
    connect(fightButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 FIGHT";
        MainMenu->hide();
        SkillMenu->show();
    });
    connect(bagButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 BAG";
        MainMenu->hide();
        ItemBagMenu->show();
    });
    connect(pokemonButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 POKEMON";
        MainMenu->hide();
        PokemonMenu->show();
    });

    SkillMenu = new QWidget(this);
    SkillMenu->setGeometry(0, 314, 525, 136); // 與原本按鈕區域重疊
    SkillMenu->hide(); // 一開始先隱藏
    QLabel* SkillMenuBackground = new QLabel(SkillMenu);
    SkillMenuBackground->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/battle/fight_skill.png"));
    SkillMenuBackground->setGeometry(0, 0, 525, 136);
    SkillMenuBackground->show();
    SkillMenuBackground->raise();

    ItemBagMenu =  new QWidget(this);
    ItemBagMenu->setGeometry(0, 314, 525, 136);
    ItemBagMenu->hide();
    QLabel* ItemBagMenu_background = new QLabel(ItemBagMenu);
    ItemBagMenu_background->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/item_bag_background.png"));
    ItemBagMenu_background->setScaledContents(true);
    ItemBagMenu_background->setGeometry(0, 0, 525, 136);
    ItemBagMenu_background->show();
    ItemBagMenu_background->raise();


    Restore_PP_Menu = new QWidget(this);
    Restore_PP_Menu->setGeometry(0, 314, 525, 136); // 與原本按鈕區域重疊
    Restore_PP_Menu->hide(); // 一開始先隱藏
    QLabel* Restore_PP_Menu_Background = new QLabel(Restore_PP_Menu);
    Restore_PP_Menu_Background->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/battle/fight_skill.png"));
    Restore_PP_Menu_Background->setGeometry(0, 0, 525, 136);
    Restore_PP_Menu_Background->show();
    Restore_PP_Menu_Background->raise();

    PokemonMenu = new QWidget(this);
    PokemonMenu->setGeometry(0, 314, 525, 136); // 與原本按鈕區域重疊
    PokemonMenu->hide(); // 一開始先隱藏
    QLabel* PokemonMenu_Background = new QLabel(PokemonMenu);
    PokemonMenu_Background->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/item_bag_background.png"));
    PokemonMenu_Background->setScaledContents(true);
    PokemonMenu_Background->setGeometry(0, 0, 525, 136);
    PokemonMenu_Background->show();
    PokemonMenu_Background->raise();


    Dialog = new QLabel(this);
    Dialog->setGeometry(19, 337, 245, 92); // (264-19 = 245, 429-337 = 92)
    Dialog->setStyleSheet("background-color: white; border: 2px solid black; font: 28px;");
    Dialog->setWordWrap(true);
    Dialog->hide();
    connect(this, &BattleScene::Attack_Dialog, this , &BattleScene::Attack_Dialog_slot);
    connect(this, &BattleScene::Items_Dialog, this , &BattleScene::Items_Dialog_slot);


    Timer = new QTimer(this);
    Timer->setSingleShot(true);
    connect(Timer, &QTimer::timeout, this, &BattleScene::ShowNextDialog);

    connect(this, &BattleScene::DialogFinished ,this ,[=](){ //每回合兩次
        UpdateBattleInfo();
        if(!End){
            PlayerTurn = !PlayerTurn;
            if(!PlayerTurn){
            QTimer::singleShot(500, this, [this]() {
                Enemy_turn();
            });
            }else{
            QTimer::singleShot(500, this, [this]() {
                Player_turn();
            });
            }
        }

    });
    // SkillMenu 上的 SkillInfo
    SkillInfo_SkillMenu = new QLabel(SkillMenu);
    SkillInfo_SkillMenu->setGeometry(397, 33, 513-397, 36);
    SkillInfo_SkillMenu->setStyleSheet("background-color: transparent; border: none ; font-size: 24px;");
    SkillInfo_SkillMenu->hide();
    SkillInfo_SkillMenu->setWordWrap(true);

    // Restore_PP_Menu 上的 SkillInfo
    SkillInfo_PPMenu = new QLabel(Restore_PP_Menu);
    SkillInfo_PPMenu->setGeometry(397, 33, 513-397, 36);
    SkillInfo_PPMenu->setStyleSheet("background-color: transparent; border: none ; font-size: 24px;");
    SkillInfo_PPMenu->hide();
    SkillInfo_PPMenu->setWordWrap(true);

    Pokeball_Animation = new QWidget(this);
    Pokeball_Animation->setGeometry(212,54,364-141,244-46);
    Pokeball_Animation->hide();
    Pokeball_Animation->setStyleSheet("background-color: transparent;");

}

void BattleScene::StartBattle() {
    End = false;
    Capture = false;
    Player_turn();
    EnemyPokemon = GenerateRandomEnemy();

    if (MyPokemon == nullptr) {
        MyPokemon = GetPokemon_From_List(0);  // 只初始化一次
    }

    MyPokemonImage = new QLabel(this);
    MyImage = QPixmap(MyPokemon->GetBackImagaePath());
    MyPokemonImage->setPixmap(MyImage.scaled(150, 150, Qt::KeepAspectRatio));
    MyPokemonImage->move(62, 200);
    MyPokemonImage->show();

    MyPokemonName = new QLabel(this);
    MyPokemonName->setText(MyPokemon->GetName());
    MyPokemonName->move(311, 225);
    MyPokemonName->setStyleSheet("font-weight: bold; font-size: 25px;");
    MyPokemonName->show();

    MyLevel = new QLabel(this);
    MyLevel->setText("Lv:"+QString::number(MyPokemon->GetLevel()));
    MyLevel->move(443, 225);
    MyLevel->setStyleSheet("font-weight: bold; font-size: 25px;");
    MyLevel->show();

    MyHp = new QLabel(this);
    MyHp->move(390, 274);
    MyHp->setText(QString::number(MyPokemon->GetCurrentHp()) + " / " + QString::number(MyPokemon->GetMaxHp()));
    MyHp->setStyleSheet("font-weight: bold; font-size: 24px;");
    MyHp->show();


    EnemyImage = new QLabel(this);
    QPixmap EnemyPix(EnemyPokemon.GetImagePath());
    EnemyImage->setPixmap(EnemyPix.scaled(150, 150, Qt::KeepAspectRatio));
    EnemyImage->setGeometry(315, 54, 150, 150);
    //EnemyImage->setScaledContents(false);
    EnemyImage->show();

    EnemyName = new QLabel(EnemyPokemon.GetName(), this);
    EnemyName->move(47, 54);
    EnemyName->setStyleSheet("font-weight: bold; font-size: 25px;");
    EnemyName->show();

    EnemyLevel = new QLabel("Lv:"+QString::number(EnemyPokemon.GetLevel()), this);
    EnemyLevel->move(170, 58);
    EnemyLevel->setStyleSheet("font-weight: bold; font-size: 25px;");
    EnemyLevel->show();

    EnemyHpBarLabel = new QLabel(this);
    UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), EnemyPokemon.GetMaxHp(), QSize(108, 10));
    EnemyHpBarLabel->move(112, 92); // 調整位置
    EnemyHpBarLabel->show();




    MyHpBarLabel = new QLabel(this);
    UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
    MyHpBarLabel->move(381, 258);
    MyHpBarLabel->show();

    RebuildAllButtons();



//    Move0_Button = new QToolButton(SkillMenu);
//    Move0_Button->setFocusPolicy(Qt::NoFocus);
//    Move0_Button->installEventFilter(this);
//    Move0_Button->setText(MyPokemon->GetMove(0));
//    Move0_Button->setGeometry(20, 30, 80, 30);
////    SkillButtons.append(Move0_Button);
//    connect(Move0_Button, &QToolButton::clicked, this, [=]() {
//        if(MyPokemon->GetCurrentPP(0)>0){
//        EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 0));
//        UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//        MyPokemon->UseMove(0);
//        SkillMenu->hide();
//        emit Attack_Dialog(0, 0);
//        }
//    });

//    if (MyPokemon->GetLevel() >= 2) {
//        Move1_Button = new QToolButton(SkillMenu);
//        Move1_Button->setFocusPolicy(Qt::NoFocus);
//        Move1_Button->installEventFilter(this);
//        Move1_Button->setText(MyPokemon->GetMove(1));
//        Move1_Button->setGeometry(20, 70, 80, 30);
////        SkillButtons.append(Move1_Button);
//        connect(Move1_Button, &QToolButton::clicked, this, [=]() {
//            if(MyPokemon->GetCurrentPP(1)>0){
//            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 1));
//            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//            MyPokemon->UseMove(1);
//            SkillMenu->hide();
//            emit Attack_Dialog(0, 1);
//            }
//        });
//    }
//    if (MyPokemon->GetLevel() >= 3) {
//        Move2_Button = new QToolButton(SkillMenu);
//        Move2_Button->setFocusPolicy(Qt::NoFocus);
//        Move2_Button->installEventFilter(this);
//        Move2_Button->setText(MyPokemon->GetMove(2));
//        Move2_Button->setGeometry(140, 30, 80, 30);
////        SkillButtons.append(Move2_Button);
//        connect(Move2_Button, &QToolButton::clicked, this, [=]() {
//            if(MyPokemon->GetCurrentPP(2)>0){
//            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 2));
//            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//            MyPokemon->UseMove(2);
//            SkillMenu->hide();
//            emit Attack_Dialog(0, 2);
//            }
//        });
//    }
//    if (MyPokemon->GetLevel() >= 4) {
//        Move3_Button = new QToolButton(SkillMenu);
//        Move3_Button->setFocusPolicy(Qt::NoFocus);
//        Move3_Button->installEventFilter(this);
//        Move3_Button->setText(MyPokemon->GetMove(3));
//        Move3_Button->setGeometry(140, 70, 80, 30);
////        SkillButtons.append(Move3_Button);
//        connect(Move3_Button, &QToolButton::clicked, this, [=]() {
//            if(MyPokemon->GetCurrentPP(3)>0){
//            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 3));
//            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//            MyPokemon->UseMove(3);
//            SkillMenu->hide();
//            emit Attack_Dialog(0, 3);
//            }
//        });
//    }
//    backButton_skill = new QToolButton(SkillMenu);
//    backButton_skill->setFocusPolicy(Qt::NoFocus);
//    backButton_skill->installEventFilter(this);
//    backButton_skill->setText("Back");
//    backButton_skill->setGeometry(240, 70, 80, 30);
////    SkillButtons.append(backButton_skill);
//    connect(backButton_skill, &QToolButton::clicked, this, [=]() {
//        SkillMenu->hide();
//        MainMenu->show();
//    });

//    //背包區


//    Pokeball_Button = new QToolButton(ItemBagMenu);
//    Pokeball_Button->setFocusPolicy(Qt::NoFocus);
//    Pokeball_Button->setText("x" + QString::number(bag->GetPokeball()));
//    Pokeball_Button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Pokeball_bag.png"));
//    Pokeball_Button->setIconSize(QSize(40, 40));
//    Pokeball_Button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    Pokeball_Button->setGeometry(40, 30, 120, 80);
//    Pokeball_Button->setStyleSheet("border: none; font-size: 18px;");

//    Potion_Button = new QToolButton(ItemBagMenu);
//    Potion_Button->setFocusPolicy(Qt::NoFocus);
//    Potion_Button->setText("x" + QString::number(bag->GetPotion()));
//    Potion_Button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Potion_bag.png"));
//    Potion_Button->setIconSize(QSize(40, 40));
//    Potion_Button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    Potion_Button->setGeometry(170, 30, 120, 80);
//    Potion_Button->setStyleSheet("border: none; font-size: 18px;");

//    Ether_Button = new QToolButton(ItemBagMenu);
//    Ether_Button->setFocusPolicy(Qt::NoFocus);
//    Ether_Button->setText("x" + QString::number(bag->GetEther()));
//    Ether_Button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Ether_bag.png"));
//    Ether_Button->setIconSize(QSize(40, 40));
//    Ether_Button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    Ether_Button->setGeometry(300, 30, 120, 80);
//    Ether_Button->setStyleSheet("border: none; font-size: 18px;");

//    Bag_BackButton = new QToolButton(ItemBagMenu);
//    Bag_BackButton->setFocusPolicy(Qt::NoFocus);
//    Bag_BackButton->setText("Back");
//    Bag_BackButton->setGeometry(430, 30, 100, 80);
//    Bag_BackButton->setStyleSheet("border: none; font-size: 24px;");

//    connect(Bag_BackButton, &QPushButton::clicked,this, [=]() {
//        ItemBagMenu->hide();
//        MainMenu->show();

//    });

//    connect(Pokeball_Button, &QPushButton::clicked,this, [=]() {
//    int pokemonCount = std::count_if(bag->Pokemon_List.begin(), bag->Pokemon_List.end(), [](const PokemonData& p) {
//        return !p.isEmpty();
//    });
//    if(bag->GetPokeball()>0 && pokemonCount<4){
//        ItemBagMenu->hide();
//        bag->UsePokeball();
//        Pokeball_Animation_Start();
//        //emit Items_Dialog(0);
//    }
//    });

//    connect(Potion_Button, &QPushButton::clicked,this, [=]() {
//    if(bag->GetPotion()>0){
//        ItemBagMenu->hide();
//        bag->UsePotion();
//        MyPokemon->RestoreHP();
//        UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//        emit Items_Dialog(1);
//        }
//    });

//    connect(Ether_Button, &QPushButton::clicked,this, [=]() {
//    if(bag->GetEther()>0){
//        ItemBagMenu->hide();

//        Restore_PP_Menu->show();

//        }
//    });

//    //恢復pp區
//    Move0_Button_PP = new QToolButton(Restore_PP_Menu);
//    Move0_Button_PP->setFocusPolicy(Qt::NoFocus);
//    Move0_Button_PP->installEventFilter(this);
//    Move0_Button_PP->setText(MyPokemon->GetMove(0));
//    Move0_Button_PP->setGeometry(20, 30, 80, 30);
////    SkillButtons.append(Move0_Button);
//    connect(Move0_Button_PP, &QToolButton::clicked, this, [=]() {
//        RestoreMoveID = 0;
//        MyPokemon->RestorePP(RestoreMoveID);
//        bag->UseEther();
//        Restore_PP_Menu->hide();
//        emit Items_Dialog(2);
//    });

//    if (MyPokemon->GetLevel() >= 2) {
//        Move1_Button_PP = new QToolButton(Restore_PP_Menu);
//        Move1_Button_PP->setFocusPolicy(Qt::NoFocus);
//        Move1_Button_PP->installEventFilter(this);
//        Move1_Button_PP->setText(MyPokemon->GetMove(1));
//        Move1_Button_PP->setGeometry(20, 70, 80, 30);
////        SkillButtons.append(Move1_Button);
//        connect(Move1_Button_PP, &QToolButton::clicked, this, [=]() {
//            RestoreMoveID = 1;
//            MyPokemon->RestorePP(RestoreMoveID);
//            bag->UseEther();
//            Restore_PP_Menu->hide();
//            emit Items_Dialog(2);
//        });
//    }
//    if (MyPokemon->GetLevel() >= 3) {
//        Move2_Button_PP = new QToolButton(Restore_PP_Menu);
//        Move2_Button_PP->setFocusPolicy(Qt::NoFocus);
//        Move2_Button_PP->installEventFilter(this);
//        Move2_Button_PP->setText(MyPokemon->GetMove(2));
//        Move2_Button_PP->setGeometry(140, 30, 80, 30);
////        SkillButtons.append(Move2_Button);
//        connect(Move2_Button_PP, &QToolButton::clicked, this, [=]() {
//            RestoreMoveID = 2;
//            MyPokemon->RestorePP(RestoreMoveID);
//            bag->UseEther();
//            Restore_PP_Menu->hide();
//            emit Items_Dialog(2);
//        });
//    }
//    if (MyPokemon->GetLevel() >= 4) {
//        Move3_Button_PP = new QToolButton(Restore_PP_Menu);
//        Move3_Button_PP->setFocusPolicy(Qt::NoFocus);
//        Move3_Button_PP->installEventFilter(this);
//        Move3_Button_PP->setText(MyPokemon->GetMove(3));
//        Move3_Button_PP->setGeometry(140, 70, 80, 30);
////        SkillButtons.append(Move3_Button);
//        connect(Move3_Button_PP, &QToolButton::clicked, this, [=]() {
//            RestoreMoveID = 3;
//            MyPokemon->RestorePP(RestoreMoveID);
//            bag->UseEther();
//            Restore_PP_Menu->hide();
//            emit Items_Dialog(2);
//        });
//    }

//    backButton_PP = new QToolButton(Restore_PP_Menu);
//    backButton_PP->setFocusPolicy(Qt::NoFocus);
//    backButton_PP->installEventFilter(this);
//    backButton_PP->setText("Back");
//    backButton_PP->setGeometry(240, 70, 80, 30);
////    SkillButtons.append(backButton_skill);
//    connect(backButton_PP, &QToolButton::clicked, this, [=]() {
//        Restore_PP_Menu->hide();
//        ItemBagMenu->show();
//    });


//    //Pokemon區
//    if(bag->Pokemon_List.size()>=1){
//        if(!bag->Pokemon_List.at(0).isEmpty()){
//        Pokemon0 = new QToolButton(PokemonMenu);
//        Pokemon0->setFocusPolicy(Qt::NoFocus);
//        Pokemon0->setIcon(QIcon(bag->Pokemon_List.at(0).GetImagePath()));
//        Pokemon0->setIconSize(QSize(40, 40));
//        Pokemon0->setText(QString::number(bag->Pokemon_List.at(0).GetCurrentHp())+" / "+QString::number(bag->Pokemon_List.at(0).GetMaxHp())+" Lv:"+QString::number(bag->Pokemon_List.at(0).GetLevel()));
//        Pokemon0->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//        Pokemon0->setGeometry(25, 30, 100, 100);
//        Pokemon0->setStyleSheet("border: none; font-size: 18px;");
//        connect(Pokemon0, &QToolButton::clicked, this, [=]() {
//            PokemonMenu->hide();
//            Pokemon_Switch_Dialog_slot(0);
//            MyPokemon = GetPokemon_From_List(0);
//            UpdateBattleInfo();
//            UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));


//        });
//        }
//    }

//    if(bag->Pokemon_List.size()>=2){
//        if(!bag->Pokemon_List.at(1).isEmpty()){
//        Pokemon1 = new QToolButton(PokemonMenu);
//        Pokemon1->setFocusPolicy(Qt::NoFocus);
//        Pokemon1->setIcon(QIcon(bag->Pokemon_List.at(1).GetImagePath()));
//        Pokemon1->setIconSize(QSize(40, 40));
//        Pokemon1->setText(QString::number(bag->Pokemon_List.at(1).GetCurrentHp())+" / "+QString::number(bag->Pokemon_List.at(1).GetMaxHp())+" Lv:"+QString::number(bag->Pokemon_List.at(1).GetLevel()));
//        Pokemon1->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//        Pokemon1->setGeometry(130, 30, 100, 100);
//        Pokemon1->setStyleSheet("border: none; font-size: 18px;");
//        connect(Pokemon1, &QToolButton::clicked, this, [=]() {
//            PokemonMenu->hide();
//            Pokemon_Switch_Dialog_slot(1);
//            MyPokemon = GetPokemon_From_List(1);
//            UpdateBattleInfo();
//            UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//        });
//        }
//    }

//    if(bag->Pokemon_List.size()>=3){
//        if(!bag->Pokemon_List.at(2).isEmpty()){
//        Pokemon2 = new QToolButton(PokemonMenu);
//        Pokemon2->setFocusPolicy(Qt::NoFocus);
//        Pokemon2->setIcon(QIcon(bag->Pokemon_List.at(2).GetImagePath()));
//        Pokemon2->setIconSize(QSize(40, 40));
//        Pokemon2->setText(QString::number(bag->Pokemon_List.at(2).GetCurrentHp())+" / "+ QString::number(bag->Pokemon_List.at(2).GetMaxHp()) +" Lv:" + QString::number(bag->Pokemon_List.at(2).GetLevel()));
//        Pokemon2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//        Pokemon2->setGeometry(235, 30, 100, 100);
//        Pokemon2->setStyleSheet("border: none; font-size: 18px;");
//        connect(Pokemon2, &QToolButton::clicked, this, [=]() {
//            PokemonMenu->hide();
//            Pokemon_Switch_Dialog_slot(2);
//            MyPokemon = GetPokemon_From_List(2);
//            UpdateBattleInfo();
//            UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
//        });
//    }
//    }

//    if(bag->Pokemon_List.size()>=4){
//        if(!bag->Pokemon_List.at(3).isEmpty()){
//        Pokemon3 = new QToolButton(PokemonMenu);
//        Pokemon3->setFocusPolicy(Qt::NoFocus);
//        Pokemon3->setIcon(QIcon(bag->Pokemon_List.at(3).GetImagePath()));
//        Pokemon3->setIconSize(QSize(40, 40));
//        Pokemon3->setText(QString::number(bag->Pokemon_List.at(3).GetCurrentHp())+" / "+QString::number(bag->Pokemon_List.at(3).GetMaxHp())+" Lv:"+QString::number(bag->Pokemon_List.at(3).GetLevel()));
//        Pokemon3->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//        Pokemon3->setGeometry(340, 30, 100, 100);
//        Pokemon3->setStyleSheet("border: none; font-size: 18px;");
//        connect(Pokemon3, &QToolButton::clicked, this, [=]() {
//            PokemonMenu->hide();
//            Pokemon_Switch_Dialog_slot(3);
//            MyPokemon = GetPokemon_From_List(3);
//            UpdateBattleInfo();
//            UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));

//        });
//    }
//    }

//    backButton_Pokemon = new QToolButton(PokemonMenu);
//    backButton_Pokemon->setFocusPolicy(Qt::NoFocus);
//    backButton_Pokemon->installEventFilter(this);
//    backButton_Pokemon->setText("Back");
//    backButton_Pokemon->setGeometry(445, 70, 40, 40);
//    connect(backButton_Pokemon, &QToolButton::clicked, this, [=]() {
//        PokemonMenu->hide();
//        MainMenu->show();
//    });







    //UpdateBattleInfo();



}
void BattleScene::UpdateHPBar(QLabel* barLabel, int currentHP, int maxHP, QSize size)
{
    if (!barLabel || maxHP <= 0) return;

    QPixmap barPixmap(size);
    barPixmap.fill(Qt::transparent);

    QPainter painter(&barPixmap);
    painter.setBrush(Qt::gray);  // 背景灰
    painter.drawRect(0, 0, size.width(), size.height());

    double ratio = static_cast<double>(currentHP) / maxHP;
    int greenWidth = static_cast<int>(size.width() * ratio);

    painter.setBrush(Qt::green);  // 血條綠
    painter.drawRect(0, 0, greenWidth, size.height());

    barLabel->setPixmap(barPixmap);

    MyHp->setText(QString::number(MyPokemon->GetCurrentHp()) + " / " + QString::number(MyPokemon->GetMaxHp()));
}

void BattleScene::UpdateBattleInfo() {
    RebuildAllButtons();

    MyImage = QPixmap(MyPokemon->GetBackImagaePath());
    MyPokemonImage->setPixmap(MyImage.scaled(150, 150, Qt::KeepAspectRatio));

    MyPokemonName->setText(MyPokemon->GetName());
    MyLevel->setText("Lv:"+QString::number(MyPokemon->GetLevel()));
    MyHp->setText(QString::number(MyPokemon->GetCurrentHp()) + " / " + QString::number(MyPokemon->GetMaxHp()));


    // 檢查敵人或玩家血量
    if (MyPokemon->GetCurrentHp() <= 0) {
        ResetBattleScene();
        End = true;
        emit BattleEnd(false);  // 玩家輸了
    } else if (EnemyPokemon.GetCurrentHp() <= 0 || Capture) {
        if(!Capture) MyPokemon->LevelUp();
        ResetBattleScene();
        End = true; //不要讓敵人下次一進場就攻擊
        emit BattleEnd(true);   // 玩家贏了

    }

}
void BattleScene::ResetBattleScene() {
    if (EnemyImage) { delete EnemyImage; EnemyImage = nullptr; }
    if (EnemyName) { delete EnemyName; EnemyName = nullptr; }
    if (EnemyLevel) { delete EnemyLevel; EnemyLevel = nullptr; }
    if (EnemyHpBarLabel) { delete EnemyHpBarLabel; EnemyHpBarLabel = nullptr; }

    if (MyPokemonImage) { delete MyPokemonImage; MyPokemonImage = nullptr; }
    if (MyPokemonName) { delete MyPokemonName; MyPokemonName = nullptr; }
    if (MyLevel) { delete MyLevel; MyLevel = nullptr; }
    if (MyHpBarLabel) { delete MyHpBarLabel; MyHpBarLabel = nullptr; }
    if (MyHp) {delete MyHp ; MyHp = nullptr; }
    RebuildAllButtons();
    PlayerTurn = true;
    Capture = false;
    EnemyPokemon.Reset();

    // 隱藏 Dialog 與選單區塊
    SkillMenu->hide();
    ItemBagMenu->hide();
    //SkillInfo_PPMenu->hide();
    //SkillInfo_SkillMenu->hide();

    PokemonMenu->hide();
    Dialog->hide();
    AttackDialogs.clear();
    ItemsDialogs.clear();
    Pokemon_Switch_Dialogs.clear();
    Timer->stop();



}
void BattleScene::RebuildAllButtons() {
    for (auto& row : Buttons) {
        for (auto* btn : row) {
            if (btn) {
                btn->removeEventFilter(this);
                delete btn;
            }
        }
    }
    Buttons.clear();
    for (int MenuIndex = 0; MenuIndex < 4; MenuIndex++) {
        QList<QToolButton*> buttons;
        for (int ButtonType = 0; ButtonType < 5; ButtonType++) {
            QToolButton* button = nullptr;

            switch (MenuIndex) {
            case 0: { // Move
                button = new QToolButton(SkillMenu);
                button->setFocusPolicy(Qt::NoFocus);
                button->installEventFilter(this);

                if (ButtonType <= 3)
                    button->setText(MyPokemon->GetMove(ButtonType));

                switch (ButtonType) {
                case 0: button->setGeometry(20, 30, 80, 30); break;
                case 1: button->setGeometry(20, 70, 80, 30); break;
                case 2: button->setGeometry(140, 30, 80, 30); break;
                case 3: button->setGeometry(140, 70, 80, 30); break;
                case 4: button->setGeometry(240, 70, 80, 30); button->setText("Back"); break;
                }

                if (ButtonType <= 3) {
                    if (MyPokemon->GetLevel() >= ButtonType + 1) button->show(); else button->hide();

                    connect(button, &QToolButton::clicked, this, [=]() {
                        if (MyPokemon->GetCurrentPP(ButtonType) > 0) {
                            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, ButtonType));
                            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
                            MyPokemon->UseMove(ButtonType);
                            SkillMenu->hide();
                            emit Attack_Dialog(0, ButtonType);
                        }
                    });
                } else {
                    connect(button, &QToolButton::clicked, this, [=]() {
                        SkillMenu->hide();
                        MainMenu->show();
                    });
                }

                break;
            }
            case 1: { // Move PP
                button = new QToolButton(Restore_PP_Menu);
                button->setFocusPolicy(Qt::NoFocus);
                button->installEventFilter(this);

                if (ButtonType <= 3)
                    button->setText(MyPokemon->GetMove(ButtonType));

                switch (ButtonType) {
                case 0: button->setGeometry(20, 30, 80, 30); break;
                case 1: button->setGeometry(20, 70, 80, 30); break;
                case 2: button->setGeometry(140, 30, 80, 30); break;
                case 3: button->setGeometry(140, 70, 80, 30); break;
                case 4: button->setGeometry(240, 70, 80, 30); button->setText("Back"); break;
                }

                if (ButtonType <= 3) {
                    if (MyPokemon->GetLevel() >= ButtonType + 1) button->show(); else button->hide();

                    connect(button, &QToolButton::clicked, this, [=]() {
                        RestoreMoveID = ButtonType;
                        MyPokemon->RestorePP(RestoreMoveID);
                        bag->UseEther();
                        Restore_PP_Menu->hide();
                        emit Items_Dialog(2);
                    });
                } else {
                    connect(button, &QToolButton::clicked, this, [=]() {
                        Restore_PP_Menu->hide();
                        ItemBagMenu->show();
                    });
                }

                break;
            }
            case 2: { // Item
                button = new QToolButton(ItemBagMenu);
                button->setFocusPolicy(Qt::NoFocus);
                button->setIconSize(QSize(40, 40));
                button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                button->setStyleSheet("border: none; font-size: 24px;");

                switch (ButtonType) {
                case 0:
                    button->setGeometry(40, 30, 120, 80);
                    button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Pokeball_bag.png"));
                    button->setText("x" + QString::number(bag->GetPokeball()));
                    connect(button, &QToolButton::clicked, this, [=]() {
                        int pokemonCount = std::count_if(bag->Pokemon_List.begin(), bag->Pokemon_List.end(), [](const PokemonData& p) {
                            return !p.isEmpty();
                        });
                        if (bag->GetPokeball() > 0 && pokemonCount < 4) {
                            ItemBagMenu->hide();
                            bag->UsePokeball();
                            Pokeball_Animation_Start();
                        }
                    });
                    break;
                case 1:
                    button->setGeometry(170, 30, 120, 80);
                    button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Potion_bag.png"));
                    button->setText("x" + QString::number(bag->GetPotion()));
                    connect(button, &QToolButton::clicked, this, [=]() {
                        if (bag->GetPotion() > 0) {
                            ItemBagMenu->hide();
                            bag->UsePotion();
                            MyPokemon->RestoreHP();
                            UpdateHPBar(MyHpBarLabel, MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
                            emit Items_Dialog(ButtonType);
                        }
                    });
                    break;
                case 2:
                    button->setGeometry(300, 30, 120, 80);
                    button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Ether_bag.png"));
                    button->setText("x" + QString::number(bag->GetEther()));
                    connect(button, &QToolButton::clicked, this, [=]() {
                        if (bag->GetEther() > 0) {
                            ItemBagMenu->hide();
                            Restore_PP_Menu->show();
                        }
                    });
                    break;
                case 3:
                    button->setGeometry(430, 30, 100, 80);
                    button->setText("Back");
                    connect(button, &QToolButton::clicked, this, [=]() {
                        ItemBagMenu->hide();
                        MainMenu->show();
                    });
                    break;
                case 4:
                    button->hide(); break;
                }

                break;
            }
            case 3: { // Pokémon
                button = new QToolButton(PokemonMenu);
                button->setFocusPolicy(Qt::NoFocus);
                button->setIconSize(QSize(40, 40));
                button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                button->setStyleSheet("border: none; font-size: 18px;");

                if (ButtonType <= 3 && bag->Pokemon_List.size() >= ButtonType + 1 && !bag->Pokemon_List.at(ButtonType).isEmpty()) {
                    button->setIcon(QIcon(bag->Pokemon_List.at(ButtonType).GetImagePath()));
                    button->setText(QString::number(bag->Pokemon_List.at(ButtonType).GetCurrentHp()) + " / " + QString::number(bag->Pokemon_List.at(ButtonType).GetMaxHp()) + " Lv:" + QString::number(bag->Pokemon_List.at(ButtonType).GetLevel()));
                } else if (ButtonType <= 3) {
                    button->hide();
                }

                switch (ButtonType) {
                case 0: button->setGeometry(25, 30, 100, 100); break;
                case 1: button->setGeometry(130, 30, 100, 100); break;
                case 2: button->setGeometry(235, 30, 100, 100); break;
                case 3: button->setGeometry(340, 30, 100, 100); break;
                case 4: {
                    button->setText("Back");
                    button->setGeometry(445, 70, 40, 40);
                    connect(button, &QToolButton::clicked, this, [=]() {
                        PokemonMenu->hide();
                        MainMenu->show();
                    });
                    break;
                }
                }

                if (ButtonType <= 3) {
                    connect(button, &QToolButton::clicked, this, [=]() {
                        PokemonMenu->hide();
                        Pokemon_Switch_Dialog_slot(ButtonType);
                        MyPokemon = GetPokemon_From_List(ButtonType);
                        UpdateBattleInfo();
                        UpdateHPBar(MyHpBarLabel, MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
                    });
                }

                break;
            }
            }

            if (button) {
                buttons.append(button);
            }
        }

        Buttons.append(buttons);
    }


}
void BattleScene::UseMove(int moveIndex) {
    Q_UNUSED(moveIndex);
}

void BattleScene::UseItem(int itemIndex) {
    Q_UNUSED(itemIndex);
}

void BattleScene::RunAway() {

    // 模擬逃跑動畫延遲或提示
    QTimer::singleShot(500, this,[this]() {
        ResetBattleScene();
        emit BattleEnd(false); // false = 沒有贏，是逃跑
    });
}


void BattleScene::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void BattleScene::ShowBattleMessage(const QString &msg) {
    Q_UNUSED(msg);
    // 未使用訊息框，保留未來可加入
}

PokemonData BattleScene::GenerateRandomEnemy() {
    int EnemyId = QRandomGenerator::global()->bounded(0, 3); // 假設 0~2 是合法寶可夢ID
    int EnemyLevel = 1; //初始皆為1級
    PokemonData enemy(EnemyId, EnemyLevel);
    qDebug() << "🎯 生成敵人 ID:" << EnemyId << " Level:" << EnemyLevel;
    return enemy;
}

PokemonData* BattleScene::GetPokemon_From_List(int id)
{
    if (bag && id >= 0 && id < bag->Pokemon_List.size()) {
        qDebug() << "✅ Bag 裡有寶可夢：" << bag->Pokemon_List.at(id).GetName();
        return &bag->Pokemon_List[id];  // 傳回的是指標
    } else {
        qDebug() << "❌ Invalid Pokemon id:" << id << " Bag 大小：" << bag->Pokemon_List.size();
        return nullptr;
    }
}

void BattleScene::Attack_Dialog_slot(int Who,int MoveID) {

    CurrentDialogIndex = 0;
    PokemonData myPokemon = *MyPokemon;
    BattleScene::Who = Who;
    AttackDialogs ={
            {
             myPokemon.GetName() + " used " + myPokemon.GetMove(MoveID) + " ! " ,
             EnemyPokemon.GetName() + " took " + QString::number(myPokemon.GetDamage(EnemyPokemon,MoveID)) + " damage!",
            "Enemy Turn"
            },
            {
            EnemyPokemon.GetName() + " used " + EnemyPokemon.GetMove(MoveID) + "!" ,
            myPokemon.GetName() + " took " + QString::number(EnemyPokemon.GetDamage(myPokemon,MoveID)) + " damage!",
            "Player Turn"
            }
    };
    if (EnemyPokemon.GetCurrentHp() <= 0)
        AttackDialogs[0].push_back("You won!");
    else if (MyPokemon->GetCurrentHp() <= 0)
        AttackDialogs[1].push_back("You Lose!");

    if (!AttackDialogs.isEmpty()) {
        Dialog->setText(AttackDialogs[Who][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(2000);
    }

}
void BattleScene::Items_Dialog_slot(int ItemID) {
    CurrentDialogIndex = 0;
    BattleScene::ItemID= ItemID;
    ItemsDialogs ={//[Item][哪個item]
                     {"Used Pokeball!" ,
                      EnemyPokemon.GetName()+" was caught. ",
                      },//[Item][哪個item][下個對話]
                     {"Used Potion!" + MyPokemon->GetName() +" restores 10 HP " ,
                      "Enemy Turn" },
                     {"Used Ether!" + MyPokemon->GetMove(RestoreMoveID) +" PP fully restored",
                      "Enemy Turn" }
                    };

    if (Capture)
        ItemsDialogs[0].push_back("You are handsome!");
    if (!ItemsDialogs.isEmpty()) {
        Dialog->setText(ItemsDialogs[ItemID][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(3000);
    }


}
void BattleScene::Pokemon_Switch_Dialog_slot(int Seletion) {
    CurrentDialogIndex = 0;

    Pokemon_Switch_Dialogs ={//[Item][哪個item]
                     {"Switch to " + bag->Pokemon_List.at(Seletion).GetName(),
                             "Enemy Turn"},//[Item][哪個item][下個對話]
//                     {"Used Potion!" + MyPokemon->GetName() +" restores 10 HP "  },
//                     {"Used Ether!" + MyPokemon->GetMove(RestoreMoveID) +" PP fully restored" }
                    };
    if (!Pokemon_Switch_Dialogs.isEmpty()) {
        Dialog->setText(Pokemon_Switch_Dialogs[0][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(3000);
    }
}
void BattleScene::ShowNextDialog() {
    CurrentDialogIndex++;

    if(!AttackDialogs.isEmpty()){
        if (CurrentDialogIndex < AttackDialogs[Who].size() ){
            if((AttackDialogs[Who][CurrentDialogIndex] == "Enemy Turn" || AttackDialogs[Who][CurrentDialogIndex] == "Player Turn")&& EnemyPokemon.GetCurrentHp() <= 0) CurrentDialogIndex++;
            Dialog->setText(AttackDialogs[Who][CurrentDialogIndex]);
            Timer->start(2000); // 繼續計時下一句
        }else {
            AttackDialogs.clear();
            Dialog->hide();
            emit DialogFinished();
        }
    }else if (!ItemsDialogs.isEmpty()){
        if (CurrentDialogIndex < ItemsDialogs[ItemID].size()) {
            Dialog->setText(ItemsDialogs[ItemID][CurrentDialogIndex]);
            Timer->start(2000); // 繼續計時下一句
        } else {
            ItemsDialogs.clear();
            Dialog->hide();
            emit DialogFinished(); // 若有需要可加上
        }
    }else if (!Pokemon_Switch_Dialogs.isEmpty()){
        if (CurrentDialogIndex < Pokemon_Switch_Dialogs[0].size()) {
            Dialog->setText(Pokemon_Switch_Dialogs[0][CurrentDialogIndex]);
            Timer->start(2000); // 繼續計時下一句
        } else {
            Pokemon_Switch_Dialogs.clear();
            Dialog->hide();
            emit DialogFinished(); // 若有需要可加上
        }
    }else{
        AttackDialogs.clear();
        ItemsDialogs.clear();
        Pokemon_Switch_Dialogs.clear();
        Dialog->hide();
    }
}
void BattleScene::Enemy_turn(){
    PokemonData myPokemon = *MyPokemon;
    int Enemy_move = QRandomGenerator::global()->bounded(0, 1);
    MyPokemon->TakeDamage(EnemyPokemon.GetDamage(myPokemon,Enemy_move)); // 技能2 傷害 30
    UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));

    emit Attack_Dialog(1,Enemy_move);

}
void BattleScene::Player_turn(){
    MainMenu->show();
}


bool BattleScene::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Enter) {
         qDebug() << "Enter event triggered";
        for (int i = 0; i < Buttons[0].size(); ++i) { // Skill menu
                    if (obj == Buttons[0][i] && i <= 3) {
                        SkillInfo_SkillMenu->setText(QString::number(MyPokemon->GetCurrentPP(i)) + " / " + QString::number(MyPokemon->GetMaxPP(i)));
                        SkillInfo_SkillMenu->show();
                        SkillInfo_SkillMenu->raise();
                    }
                }
        for (int i = 0; i < Buttons[1].size(); ++i) { // Restore PP menu
                    if (obj == Buttons[1][i] && i <= 3) {
                        SkillInfo_PPMenu->setText(QString::number(MyPokemon->GetCurrentPP(i)) + " / " + QString::number(MyPokemon->GetMaxPP(i)));
                        SkillInfo_PPMenu->show();
                        SkillInfo_PPMenu->raise();
                    }
                }
    } else if (event->type() == QEvent::Leave) {
        qDebug() << "Leave event triggered";
        SkillInfo_SkillMenu->hide();
        SkillInfo_PPMenu->hide();
    }
    return QWidget::eventFilter(obj, event);
}

void BattleScene::Pokeball_Animation_Start() {
    Pokeball_Animation->show();

    QLabel* Small_Pokeball = new QLabel(Pokeball_Animation);
    Small_Pokeball->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/icon/Pokeball_bag.png"));
    Small_Pokeball->setScaledContents(true);
    Small_Pokeball->setGeometry(0, 108, 24, 24);
    Small_Pokeball->show();
    Small_Pokeball->raise();

    QTimer* timer = new QTimer(Pokeball_Animation);
    int frame = 0;
    int originalW = 150;
    int originalH = 150;

    connect(timer, &QTimer::timeout, Pokeball_Animation, [=]() mutable {
        frame++;
        int x = frame;
        int y = 0.01 * (frame - 100) * (frame - 100);
        if (frame < 100) {
            Small_Pokeball->move(x, y);
        }
        if (frame >= 100 && frame <= 140) {
            double scale = 1.0 - (frame - 100) / 40.0;
            int newW = static_cast<int>(originalW * scale);
            int newH = static_cast<int>(originalH * scale);
            qDebug() << "Scale:" << scale << "Size:" << newW << "x" << newH;
            EnemyImage->setGeometry(315, 54, newW, newH);
            EnemyImage->setScaledContents(true);
        }

        if (frame >= 160) {  // End the animation
            timer->stop();
            Pokeball_Animation->hide();
            delete Small_Pokeball;
            CapturePokemon();

        }
    });

    timer->start(16);
}
void BattleScene::CapturePokemon(){
    bag->Add_Pokemon(EnemyPokemon);
    Capture = true;
    emit Items_Dialog(0);
}
