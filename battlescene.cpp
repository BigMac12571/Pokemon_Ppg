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

    connect(this, &BattleScene::DialogFinished ,this ,[=](){ //每回合兩次
        UpdateBattleInfo();
        UpdateBagUI();
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
    SkillInfo = new QLabel(SkillMenu);  // 或 this
    SkillInfo->setGeometry(397, 35, 513-397, 36);  // 位置根據 SkillMenu 調整
    SkillInfo->setStyleSheet("background-color: transparent; border: none ; font-size: 24px;");
    SkillInfo->setVisible(false);
    SkillInfo->setWordWrap(true);

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




    Move0_Button = new QToolButton(SkillMenu);
    Move0_Button->setFocusPolicy(Qt::NoFocus);
    Move0_Button->installEventFilter(this);
    Move0_Button->setText(MyPokemon->GetMove(0));
    Move0_Button->setGeometry(20, 30, 80, 30);
//    SkillButtons.append(Move0_Button);
    connect(Move0_Button, &QToolButton::clicked, this, [=]() {
        EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 0));
        UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
        MyPokemon->UseMove(0);
        SkillMenu->hide();
        emit Attack_Dialog(0, 0);
    });

    if (MyPokemon->GetLevel() >= 2) {
        Move1_Button = new QToolButton(SkillMenu);
        Move1_Button->setFocusPolicy(Qt::NoFocus);
        Move1_Button->installEventFilter(this);
        Move1_Button->setText(MyPokemon->GetMove(1));
        Move1_Button->setGeometry(20, 70, 80, 30);
//        SkillButtons.append(Move1_Button);
        connect(Move1_Button, &QToolButton::clicked, this, [=]() {
            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 1));
            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
            MyPokemon->UseMove(1);
            SkillMenu->hide();
            emit Attack_Dialog(0, 1);
        });
    }
    if (MyPokemon->GetLevel() >= 3) {
        Move2_Button = new QToolButton(SkillMenu);
        Move2_Button->setFocusPolicy(Qt::NoFocus);
        Move2_Button->installEventFilter(this);
        Move2_Button->setText(MyPokemon->GetMove(2));
        Move2_Button->setGeometry(140, 30, 80, 30);
//        SkillButtons.append(Move2_Button);
        connect(Move2_Button, &QToolButton::clicked, this, [=]() {
            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 2));
            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
            MyPokemon->UseMove(2);
            SkillMenu->hide();
            emit Attack_Dialog(0, 2);
        });
    }
    if (MyPokemon->GetLevel() >= 4) {
        Move3_Button = new QToolButton(SkillMenu);
        Move3_Button->setFocusPolicy(Qt::NoFocus);
        Move3_Button->installEventFilter(this);
        Move3_Button->setText(MyPokemon->GetMove(3));
        Move3_Button->setGeometry(140, 70, 80, 30);
//        SkillButtons.append(Move3_Button);
        connect(Move3_Button, &QToolButton::clicked, this, [=]() {
            EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, 3));
            UpdateHPBar(EnemyHpBarLabel, EnemyPokemon.GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
            MyPokemon->UseMove(3);
            SkillMenu->hide();
            emit Attack_Dialog(0, 3);
        });
    }
    backButton_skill = new QToolButton(SkillMenu);
    backButton_skill->setFocusPolicy(Qt::NoFocus);
    backButton_skill->installEventFilter(this);
    backButton_skill->setText("Back");
    backButton_skill->setGeometry(240, 70, 80, 30);
//    SkillButtons.append(backButton_skill);
    connect(backButton_skill, &QToolButton::clicked, this, [=]() {
        SkillMenu->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();
    });

    //背包區


    Pokeball_Button = new QToolButton(bag_area);
    Pokeball_Button->setText("x" + QString::number(bag->GetPokeball()));
    Pokeball_Button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Pokeball_bag.png"));
    Pokeball_Button->setIconSize(QSize(40, 40));
    Pokeball_Button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Pokeball_Button->setGeometry(40, 30, 120, 80);
    Pokeball_Button->setStyleSheet("border: none; font-size: 18px;");

    Potion_Button = new QToolButton(bag_area);
    Potion_Button->setText("x" + QString::number(bag->GetPotion()));
    Potion_Button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Potion_bag.png"));
    Potion_Button->setIconSize(QSize(40, 40));
    Potion_Button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Potion_Button->setGeometry(170, 30, 120, 80);
    Potion_Button->setStyleSheet("border: none; font-size: 18px;");

    Ether_Button = new QToolButton(bag_area);
    Ether_Button->setText("x" + QString::number(bag->GetEther()));
    Ether_Button->setIcon(QIcon(":/new/prefix1/Dataset/Image/icon/Ether_bag.png"));
    Ether_Button->setIconSize(QSize(40, 40));
    Ether_Button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Ether_Button->setGeometry(300, 30, 120, 80);
    Ether_Button->setStyleSheet("border: none; font-size: 18px;");

    Bag_BackButton = new QToolButton(bag_area);
    Bag_BackButton->setText("Back");
    Bag_BackButton->setGeometry(430, 30, 100, 80);
    Bag_BackButton->setStyleSheet("border: none; font-size: 18px;");

    connect(Bag_BackButton, &QPushButton::clicked,this, [=]() {
        bag_area->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();

    });

    connect(Pokeball_Button, &QPushButton::clicked,this, [=]() {
    if(bag->GetPokeball()>0){
        bag_area->hide();
        bag->UsePokeball();
        emit Items_Dialog(0);
    }
    });

    connect(Potion_Button, &QPushButton::clicked,this, [=]() {
    if(bag->GetPotion()>0){
        bag_area->hide();
        bag->UsePotion();
        MyPokemon->RestoreHP();
        UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
        emit Items_Dialog(1);
        }
    });

    connect(Ether_Button, &QPushButton::clicked,this, [=]() {
    if(bag->GetEther()>0){
        bag_area->hide();
        bag->UseEther();
        emit Items_Dialog(2);
        }
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
    Pokeball_Button->setText("x" + QString::number(bag->GetPokeball()));
    Potion_Button->setText("x" + QString::number(bag->GetPotion()));
    Ether_Button->setText("x" + QString::number(bag->GetEther()));
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

    QList<QToolButton*> old_skill_Buttons = SkillMenu->findChildren<QToolButton*>();
    for (QToolButton* btn : old_skill_Buttons) {
        btn->deleteLater();
    }
    QList<QToolButton*> old_bag_Buttons = bag_area->findChildren<QToolButton*>();
    for (QToolButton* btn : old_bag_Buttons) {
        btn->deleteLater();
    }
    PlayerTurn = true;
    EnemyPokemon.Reset();

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
            }
    };
    if (EnemyPokemon.GetCurrentHp() <= 0)
        AttackDialogs[0].push_back("You won!");
    else if (MyPokemon->GetCurrentHp() <= 0)
        AttackDialogs[1].push_back("You Lose!");

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
                     {"Used Ether!" + MyPokemon->GetMove(0) +" PP fully restored" }
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
        if (CurrentDialogIndex < AttackDialogs[Who].size()){
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

    emit Attack_Dialog(1,Enemy_move);

}
void BattleScene::Player_turn(){
    fightButton->show();
    bagButton->show();
    pokemonButton->show();
    runButton->show();
}

void BattleScene::UpdateBagUI() {

}
//void BattleScene::keyPressEvent(QKeyEvent *event) {
//    if (!SkillMenu->isVisible()) return;

//    int TotalButtons = SkillButtons.size();
//    int row = currentSkillIndex / 3;  // 我們有3列（Move0、Move1、Move2、Move3、Back）
//    int col = currentSkillIndex % 3;  // 列是3

//    switch (event->key()) {
//        case Qt::Key_Up:
//            if (row > 0) row--;
//            break;
//        case Qt::Key_Down:
//            if ((row + 1) * 3 + col < TotalButtons)
//                row++;
//            break;
//        case Qt::Key_Left:
//            if (col > 0) col--;
//            break;
//        case Qt::Key_Right:
//            if (col < 2 && row * 3 + (col + 1) < TotalButtons) // 限制右邊不能超過2
//                col++;
//            break;
//        case Qt::Key_Return:
//        case Qt::Key_Enter:
//            if (currentSkillIndex < TotalButtons)
//                SkillButtons[currentSkillIndex]->click();
//            return;
//        default:
//            return;
//    }

//    int newIndex = row * 3 + col;  // 根據行列計算新索引
//    if (newIndex < TotalButtons) {
//        SkillButtons[currentSkillIndex]->setStyleSheet("border: none;");
//        currentSkillIndex = newIndex;
//        SkillButtons[currentSkillIndex]->setStyleSheet("border: 2px solid yellow;");
//    }
//     event->accept();
//}
bool BattleScene::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Enter) {
        if (obj == Move0_Button) {
            SkillInfo->setText(QString::number(MyPokemon->GetCurrentPP(0))+" / "+ QString::number(MyPokemon->GetMaxPP(0)));
            SkillInfo->setVisible(true);
        } else if (obj == Move1_Button) {
            SkillInfo->setText(QString::number(MyPokemon->GetCurrentPP(1))+" / "+ QString::number(MyPokemon->GetMaxPP(1)));
            SkillInfo->setVisible(true);
        } else if (obj == Move2_Button) {
            SkillInfo->setText(QString::number(MyPokemon->GetCurrentPP(2))+" / "+ QString::number(MyPokemon->GetMaxPP(2)));
            SkillInfo->setVisible(true);
        } else if (obj == Move3_Button) {
            SkillInfo->setText(QString::number(MyPokemon->GetCurrentPP(3))+" / "+ QString::number(MyPokemon->GetMaxPP(3)));
            SkillInfo->setVisible(true);
        }
    } else if (event->type() == QEvent::Leave) {
        SkillInfo->setVisible(false);
    }
    return QWidget::eventFilter(obj, event);
}
