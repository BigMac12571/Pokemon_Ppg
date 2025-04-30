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


    // 建立四個指令選單按鈕
    fightButton = new QPushButton("FIGHT", this);
    bagButton = new QPushButton("BAG", this);
    pokemonButton = new QPushButton("POKEMON", this);
    runButton = new QPushButton("RUN", this);

    // 位置模擬 GBA 排列
    fightButton->setGeometry(290, 345, 100, 30);
    bagButton->setGeometry(400, 345, 100, 30);
    pokemonButton->setGeometry(290, 385, 100, 30);
    runButton->setGeometry(400, 385, 100, 30);

    connect(runButton, &QPushButton::clicked, this, &BattleScene::RunAway);
    connect(fightButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 FIGHT";
        fightButton->hide();
        bagButton->hide();
        pokemonButton->hide();
        runButton->hide();
        SkillMenu->show();
    });
    connect(bagButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 BAG";
        fightButton->hide();
        bagButton->hide();
        pokemonButton->hide();
        runButton->hide();
        bag_area->show();
    });
    connect(pokemonButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 POKEMON";
    });

    SkillMenu = new QWidget(this);
    SkillMenu->setGeometry(0, 314, 525, 136); // 與原本按鈕區域重疊
    SkillMenu->hide(); // 一開始先隱藏
    QLabel* SkillMenuBackground = new QLabel(SkillMenu);
    SkillMenuBackground->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/battle/fight_skill.png"));
    SkillMenuBackground->setGeometry(0, 0, 525, 136);
    SkillMenuBackground->show();
    SkillMenuBackground->raise();

    bag_area =  new QWidget(this);
    bag_area->setGeometry(0, 314, 525, 136);
    bag_area->hide();
    QLabel* bag_area_background = new QLabel(bag_area);
    bag_area_background->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/item_bag_background.png"));
    bag_area_background->setScaledContents(true);
    bag_area_background->setGeometry(0, 0, 525, 136);
    bag_area_background->show();
    bag_area_background->raise();

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

    connect(this, &BattleScene::DialogFinished ,this ,[=](){
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

}

void BattleScene::StartBattle() {
    End = false;
    Player_turn();
    EnemyPokemon = GenerateRandomEnemy();

    EnemyImage = new QLabel(this);
    QPixmap pix(EnemyPokemon.GetImagePath());
    EnemyImage->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio));
    EnemyImage->move(315, 54);
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

    if (MyPokemon == nullptr) {
        MyPokemon = GetPokemon_From_List(0);  // 只初始化一次
    }

    MyPokemonImage = new QLabel(this);
    QPixmap myImage(MyPokemon->GetBackImagaePath());
    MyPokemonImage->setPixmap(myImage.scaled(150, 150, Qt::KeepAspectRatio));
    MyPokemonImage->move(62, 209);
    MyPokemonImage->show();

    MyPokemonName = new QLabel(MyPokemon->GetName(), this);
    MyPokemonName->move(311, 225);
    MyPokemonName->setStyleSheet("font-weight: bold; font-size: 25px;");
    MyPokemonName->show();

    MyLevel = new QLabel("Lv:"+QString::number(MyPokemon->GetLevel()), this);
    MyLevel->move(443, 225);
    MyLevel->setStyleSheet("font-weight: bold; font-size: 25px;");
    MyLevel->show();

    MyHpBarLabel = new QLabel(this);
    UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
    MyHpBarLabel->move(381, 258);
    MyHpBarLabel->show();


    // 技能選單區


    QPushButton* backButton_skill = new QPushButton("Back", SkillMenu);
    backButton_skill->setGeometry(240, 70, 80, 30);
    connect(backButton_skill, &QPushButton::clicked,this, [=]() {
        SkillMenu->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();


    });




    QPushButton* Move0_Button = new QPushButton(MyPokemon->GetMove(0), SkillMenu);
    Move0_Button->setGeometry(20, 30, 80, 30);
    connect(Move0_Button, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon,0)); // 技能1 傷害 20
        UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));


        SkillMenu->hide();
        emit Attack_Dialog(0,0);
    });

    if(MyPokemon->GetLevel()>=2){
    QPushButton* Move1_Button = new QPushButton(MyPokemon->GetMove(1), SkillMenu);
    Move1_Button->setGeometry(20, 70, 80, 30);
    connect(Move1_Button, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon,1)); // 技能2 傷害 30
        UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));


        SkillMenu->hide();
        emit Attack_Dialog(0,1);
    });
    }
    if(MyPokemon->GetLevel()>=3){
    QPushButton* Move2_Button = new QPushButton(MyPokemon->GetMove(2), SkillMenu);
    Move2_Button->setGeometry(140, 30, 80, 30);
    connect(Move2_Button, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon,2)); // 普通攻擊傷害 10
        UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));


        SkillMenu->hide();
        emit Attack_Dialog(0,2);
    });
    }
    if(MyPokemon->GetLevel()>=4){
    QPushButton* Move3_Button = new QPushButton(MyPokemon->GetMove(3), SkillMenu);
    Move3_Button->setGeometry(140, 70, 80, 30);
    connect(Move3_Button, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon,3)); // 普通攻擊傷害 10
        UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));


        SkillMenu->hide();
        emit Attack_Dialog(0,3);
    });
    }
    //背包區


    QPushButton* Pokeball_Button = new QPushButton(bag_area);
    QPushButton* Potion_Button = new QPushButton(bag_area);
    QPushButton* Ether_Button = new QPushButton(bag_area);
    QPushButton* backButton_bag_Button = new QPushButton("Back", bag_area);

    QIcon Pokeball_icon(":/new/prefix1/Dataset/Image/icon/Pokeball_bag.png");
    Pokeball_Button->setIcon(Pokeball_icon);
    Pokeball_Button->setGeometry(40, 30, 80, 80);
    Pokeball_Button->setIconSize(Pokeball_Button->size());
    Pokeball_Button->setFlat(true);  // 讓按鈕背景變透明、沒有立體效果
    Pokeball_Button->setStyleSheet("border: none;");  // 移除邊框

    QIcon Potion_icon(":/new/prefix1/Dataset/Image/icon/Potion_bag.png");
    Potion_Button->setIcon(Potion_icon);
    Potion_Button->setGeometry(150, 30, 80, 80);
    Potion_Button->setIconSize(Potion_Button->size());
    Potion_Button->setFlat(true);
    Potion_Button->setStyleSheet("border: none;");

    QIcon Ether_icon(":/new/prefix1/Dataset/Image/icon/Ether_bag.png");
    Ether_Button->setIcon(Ether_icon);
    Ether_Button->setGeometry(270, 30, 80, 80);
    Ether_Button->setIconSize(Ether_Button->size());
    Ether_Button->setFlat(true);
    Ether_Button->setStyleSheet("border: none;");

    backButton_bag_Button->setGeometry(380, 30, 80, 80);
    backButton_bag_Button->setStyleSheet("border: none;");

    connect(backButton_bag_Button, &QPushButton::clicked,this, [=]() {
        bag_area->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();

    });

    connect(Pokeball_Button, &QPushButton::clicked,this, [=]() {

        bag_area->hide();

        emit Items_Dialog(0);
    });

    connect(Potion_Button, &QPushButton::clicked,this, [=]() {

        bag_area->hide();

        emit Items_Dialog(1);
    });

    connect(Ether_Button, &QPushButton::clicked,this, [=]() {

        bag_area->hide();

        emit Items_Dialog(2);
    });




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
}

void BattleScene::UpdateBattleInfo() {
    // 檢查敵人或玩家血量
    if (MyPokemon->GetCurrentHp() <= 0) {
        ResetBattleScene();
        End = true;
        emit BattleEnd(false);  // 玩家輸了
    } else if (EnemyPokemon.GetCurrentHp() <= 0) {
        ResetBattleScene();
        MyPokemon->LevelUp();
        End = true;
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

    QList<QPushButton*> oldButtons = SkillMenu->findChildren<QPushButton*>();
    for (QPushButton* btn : oldButtons) {
        btn->deleteLater();
    }
    PlayerTurn = true;
    EnemyPokemon.Reset();  // 依你的建構子方式給預設值

    // 隱藏 Dialog 與選單區塊
    SkillMenu->hide();
    bag_area->hide();
    Dialog->hide();
    AttackDialogs.clear();
    ItemsDialogs.clear();
    Timer->stop();

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
//void BattleScene::BattleEnd(bool win) {

//    if (win) {
//        qDebug() << "You win!";
//        myPokemon.LevelUp();
//    } else {
//        qDebug() << "You lose!";
//    }
//    // 你可以在這裡結束戰鬥畫面、切換場景、恢復狀態等
//}


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
             "You won"
            },
            {
            EnemyPokemon.GetName() + " used " + EnemyPokemon.GetMove(MoveID) + "!" ,
            myPokemon.GetName() + " took " + QString::number(EnemyPokemon.GetDamage(myPokemon,MoveID)) + " damage!",
             "Enemy won"
            }
    };
    if (EnemyPokemon.GetCurrentHp() <= 0)
        AttackDialogs[0].push_back("You won!");
    else if (MyPokemon->GetCurrentHp() <= 0)
        AttackDialogs[1].push_back("Enemy won!");

    if (!AttackDialogs.isEmpty()) {
        Dialog->setText(AttackDialogs[Who][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(3000);
    }

}
void BattleScene::Items_Dialog_slot(int ItemID) {
    CurrentDialogIndex = 0;
    BattleScene::ItemID= ItemID;
    ItemsDialogs ={//[Item][哪個item]
                     {"Used Pokeball!" ,
                      EnemyPokemon.GetName()+" was caught. "},//[Item][哪個item][下個對話]
                     {"Used Potion!" + MyPokemon->GetName() +" restores 10 HP "  },
                     {"Used Ether!" + MyPokemon->GetMove(ItemID) +" PP fully restored" }
                    };
    if (!ItemsDialogs.isEmpty()) {
        Dialog->setText(ItemsDialogs[ItemID][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(3000);
    }


}
void BattleScene::ShowNextDialog() {
    CurrentDialogIndex++;

    if(!AttackDialogs.isEmpty()){
        if (CurrentDialogIndex < AttackDialogs[Who].size()-1){
            Dialog->setText(AttackDialogs[Who][CurrentDialogIndex]);
            Timer->start(3000); // 繼續計時下一句
        }else {
            AttackDialogs.clear();
            Dialog->hide();
            emit DialogFinished(); // 若有需要可加上
        }
    }else if (!ItemsDialogs.isEmpty()){
        if (CurrentDialogIndex < ItemsDialogs[ItemID].size()) {
            Dialog->setText(ItemsDialogs[ItemID][CurrentDialogIndex]);
            Timer->start(3000); // 繼續計時下一句
        } else {
            ItemsDialogs.clear();
            Dialog->hide();
            emit DialogFinished(); // 若有需要可加上
        }
    }else{
        AttackDialogs.clear();
        ItemsDialogs.clear();
        Dialog->hide();
    }
}
void BattleScene::Enemy_turn(){
    PokemonData myPokemon = *MyPokemon;
    int Enemy_move = QRandomGenerator::global()->bounded(0, 1);
    MyPokemon->TakeDamage(EnemyPokemon.GetDamage(myPokemon,Enemy_move)); // 技能2 傷害 30
    UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
    UpdateBattleInfo();
    emit Attack_Dialog(1,Enemy_move);

}
void BattleScene::Player_turn(){
    fightButton->show();
    bagButton->show();
    pokemonButton->show();
    runButton->show();
}
