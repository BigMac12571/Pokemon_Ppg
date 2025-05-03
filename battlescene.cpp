#include "battlescene.h"
#include <QDebug>
#include <QTimer>
#include <QSoundEffect>


BattleScene::BattleScene(Bag *myBag, QWidget *parent)
    : QWidget(parent), bag(myBag) , PlayerTurn(true),MyPokemon(nullptr),EnemyPokemon(0,1)
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

    SpecialMenu = new QWidget(this);
    SpecialMenu->setGeometry(0, 314, 525, 136); // 與原本按鈕區域重疊
    SpecialMenu->hide(); // 一開始先隱藏
    QLabel* SpecialMenuBackground = new QLabel(SpecialMenu);
    SpecialMenuBackground->setPixmap(QPixmap(":/new/prefix1/Dataset/Image/battle/fight_skill.png"));
    SpecialMenuBackground->setGeometry(0, 0, 525, 136);
    SpecialMenuBackground->show();
    SpecialMenuBackground->raise();

    Dialog = new QLabel(this);
    Dialog->setGeometry(19, 337, 245, 92); // (264-19 = 245, 429-337 = 92)
    Dialog->setStyleSheet( "background-color: rgba(0,0,0,0);" "color: white;""font-size: 24px;" );
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

    for(int i = 0 ; i < 3; i++){
        QLabel* Type = new QLabel(SkillMenu);
        Type->setGeometry(420, 59, 40,40);
        Type->setStyleSheet("background-color: transparent; border: none");

        QPixmap Type_image;
        switch(i){
        case 0: Type_image.load(":/new/prefix1/Dataset/Image/type/Grass.jpg"); break;
        case 1: Type_image.load(":/new/prefix1/Dataset/Image/type/Water.jpg"); break;
        case 2: Type_image.load(":/new/prefix1/Dataset/Image/type/Fire.jpg"); break;
        }

        Type->setPixmap(Type_image.scaled(Type->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        Type_SkillMenu.push_back(Type);
    }

    for(int i = 0 ; i < 3; i++){
        QLabel* Type = new QLabel(Restore_PP_Menu);
        Type->setGeometry(420, 59, 40,40);
        Type->setStyleSheet("background-color: transparent; border: none");

        QPixmap Type_image;
        switch(i){
        case 0: Type_image.load(":/new/prefix1/Dataset/Image/type/Grass.jpg"); break;
        case 1: Type_image.load(":/new/prefix1/Dataset/Image/type/Water.jpg"); break;
        case 2: Type_image.load(":/new/prefix1/Dataset/Image/type/Fire.jpg"); break;
        }

        Type->setPixmap(Type_image.scaled(Type->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        Type_PPMenu.push_back(Type);
    }
    Pokeball_Animation = new QWidget(this);
    Pokeball_Animation->setGeometry(212,54,364-141,244-46);
    Pokeball_Animation->hide();
    Pokeball_Animation->setStyleSheet("background-color: transparent;");


    battleSound = new QSoundEffect(this);
    battleSound->setSource(QUrl::fromLocalFile("C:/Qt/pokemon/Pokemon_Ppg/battle_start.wav"));
    battleSound->setLoopCount(1);
    battleSound->setVolume(0.5);

    connect(battleSound, &QSoundEffect::statusChanged, this, [=]() {
        qDebug() << "Sound status:" << battleSound->status();
    });

}

void BattleScene::StartBattle() {

    End = false;


    Capture = false;
    MainMenu->hide();

    qDebug() <<"創建MyPokemon當掉";
    if (MyPokemon == nullptr) {
        MyPokemon = GetPokemon_From_List(0);  // 只初始化一次
    }
     qDebug() <<"創建MyPokemon當掉";
    //for(int i=0; i< 3; i++) qDebug() <<MyPokemon->GetCurrentSpecialatk_remaing_times(i);
    MyPokemon->SetSpec();
    //for(int i=0; i< 3; i++) qDebug() <<MyPokemon->GetCurrentSpecialatk_remaing_times(i);

    for(int i=0; i< 5; i++) MyPokemon->LevelUp();
    qDebug() <<"創建EnemyPokemon當掉";
    EnemyPokemon = GenerateRandomEnemy();
    qDebug() <<"創建EnemyPokemon當掉";


    battleSound->play();



    MyPokemonImage = new QLabel(this);
    MyImage = QPixmap(MyPokemon->GetBackImagaePath());
    MyPokemonImage->setPixmap(MyImage.scaled(150, 150, Qt::KeepAspectRatio));
    MyPokemonImage->move(62, 200);
    MyPokemonImage->show();

    MyPokemonName = new QLabel(this);
    MyPokemonName->setText(MyPokemon->GetName());
    MyPokemonName->move(311, 225);
    MyPokemonName->setFixedWidth(150);
    MyPokemonName->setStyleSheet("font-weight: bold; font-size: 20px;");
    MyPokemonName->show();

    MyLevel = new QLabel(this);
    MyLevel->setText("Lv:"+QString::number(MyPokemon->GetLevel()));
    MyLevel->move(443, 225);
    MyLevel->setFixedWidth(100);
    MyLevel->setStyleSheet("font-weight: bold; font-size: 20px;");
    MyLevel->show();

    MyHp = new QLabel(this);
    MyHp->move(390, 277);
    MyHp->setText(QString::number(MyPokemon->GetCurrentHp()) + " / " + QString::number(MyPokemon->GetMaxHp()));
    MyHp->setFixedWidth(100);
    MyHp->setStyleSheet("font-weight: bold; font-size: 20px;");
    MyHp->show();

    for(int i = 0 ; i < 3; i++){
        if(i == MyPokemon->GetType()) {
            Type_SkillMenu.at(MyPokemon->GetType())->show();
            Type_PPMenu.at(MyPokemon->GetType())->show();
        }else{
        Type_SkillMenu.at(i)->hide();
        Type_PPMenu.at(i)->hide();
        }
    }


    EnemyImage = new QLabel(this);
    QPixmap EnemyPix(EnemyPokemon.GetImagePath());
    EnemyImage->setPixmap(EnemyPix.scaled(150, 150, Qt::KeepAspectRatio));
    EnemyImage->setGeometry(315, 54, 150, 150);
    //EnemyImage->setScaledContents(false);
    EnemyImage->show();

    EnemyName = new QLabel(EnemyPokemon.GetName(), this);
    EnemyName->move(47, 60);
    EnemyName->setStyleSheet("font-weight: bold; font-size: 20px;");
    EnemyName->show();

    EnemyLevel = new QLabel("Lv:"+QString::number(EnemyPokemon.GetLevel()), this);
    EnemyLevel->move(170, 58);
    EnemyLevel->setStyleSheet("font-weight: bold; font-size: 20px;");
    EnemyLevel->show();

    EnemyHpBarLabel = new QLabel(this);
    UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), EnemyPokemon.GetMaxHp(), QSize(108, 10));
    EnemyHpBarLabel->move(112, 92); // 調整位置
    EnemyHpBarLabel->show();

    MyHpBarLabel = new QLabel(this);
    UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
    MyHpBarLabel->move(381, 258);
    MyHpBarLabel->show();

    if(MyPokemon->GetSpeed() >= EnemyPokemon.GetSpeed()){
        Dialog->show();
        Dialog->setText(MyPokemon->GetName() + "'s Speed("+QString::number(MyPokemon->GetSpeed())+ ") is greater than (or equal to) " +EnemyPokemon.GetName()+"'s Speed("+ QString::number(EnemyPokemon.GetSpeed())+")");
        QTimer::singleShot(4000, this, [this]() {
            Dialog->setText("Player turn");
            QTimer::singleShot(2000, this, [this]() {
                Dialog->hide();
                Player_turn();
                RebuildAllButtons();
            });
        });

    }
    else {

        Dialog->show();
        Dialog->setText(EnemyPokemon.GetName() + "'s Speed("+QString::number(EnemyPokemon.GetSpeed())+ ") is greater than" +MyPokemon->GetName()+"'s Speed("+QString::number(MyPokemon->GetSpeed())+")");
        QTimer::singleShot(4000, this, [this]() {
            Dialog->setText("Enemy turn");
            QTimer::singleShot(2000, this, [this]() {
                Dialog->hide();
                Enemy_turn();
                RebuildAllButtons();
            });
        });







    }

    qDebug() <<"123";

    qDebug() <<"123";

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

    for(int i = 0 ; i < 3; i++){
        if(i == MyPokemon->GetType()) {
            Type_SkillMenu.at(MyPokemon->GetType())->show();
            Type_SkillMenu.at(MyPokemon->GetType())->raise();
            Type_PPMenu.at(MyPokemon->GetType())->show();
        }else{
            Type_SkillMenu.at(i)->hide();
            Type_PPMenu.at(i)->hide();
            }
    }


    // 檢查敵人或玩家血量

    if (EnemyPokemon.GetCurrentHp() <= 0 || Capture) {
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
    //RebuildAllButtons();
    PlayerTurn = true;
    Capture = false;
    //EnemyPokemon.Reset();

    // 隱藏 Dialog 與選單區塊
    SkillMenu->hide();
    ItemBagMenu->hide();
    PokemonMenu->hide();
    SpecialMenu->hide();
    //SkillInfo_PPMenu->hide();
    //SkillInfo_SkillMenu->hide();

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
    for (int MenuIndex = 0; MenuIndex < 5; MenuIndex++) {
        QList<QToolButton*> buttons;
        qDebug() <<MenuIndex;
        for (int ButtonType = 0; ButtonType < 5; ButtonType++) {
            qDebug() <<ButtonType;
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
                            UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), EnemyPokemon.GetMaxHp(), QSize(108, 10));
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
                if( ButtonType == 4){
                    QToolButton* Special = new QToolButton(SkillMenu);
                    Special->setFocusPolicy(Qt::NoFocus);
                    Special->installEventFilter(this);
                    Special->setGeometry(240, 30, 80, 30);
                    Special->setText("Special");
                    buttons.append(Special);
                    connect(Special, &QToolButton::clicked, this, [=]() {
                        SkillMenu->hide();
                        SpecialMenu->show();
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
            case 2: {
                button = new QToolButton(SpecialMenu);
                button->setFocusPolicy(Qt::NoFocus);
                button->installEventFilter(this);
                //button->setFixedSize(80, 80);
                //button->setStyleSheet( "color: white;");

                if (ButtonType <= 2)
                {button->setText(MyPokemon->GetSpecialATK(ButtonType)+ "\nRemaining: " + QString::number(MyPokemon->GetCurrentSpecialatk_remaing_times(ButtonType)));}

                switch (ButtonType) {
                case 0: button->setGeometry(20, 30, 80, 35);   break;
                case 1: button->setGeometry(20, 70, 80, 35);break;
                case 2: button->setGeometry(140, 30, 80, 35); break;
                case 3: button->setGeometry(240, 70, 80, 35); button->setText("Back"); break;
                case 4: button->setGeometry(362, 30, 152, 76); break;
                default :button->hide(); break;
                }

                if (ButtonType <= 2) {

                    if (MyPokemon->GetLevel() >= ButtonType *2 +1 && MyPokemon->GetCurrentSpecialatk_remaing_times(ButtonType) >= 1) button->show(); else button->hide();

                    connect(button, &QToolButton::clicked, SpecialMenu, [=]() {
                            MyPokemon->UseSpec(ButtonType);
                            SpecialMenu->hide();
                            int t = 0;
                            QTimer* timer = new QTimer(SpecialMenu);
                            connect(timer, &QTimer::timeout, SpecialMenu, [=]() mutable {

                                EnemyPokemon.TakeDamage(MyPokemon->GetDamage(EnemyPokemon, -1 ,ButtonType));
                                UpdateHPBar(EnemyHpBarLabel,EnemyPokemon.GetCurrentHp(), EnemyPokemon.GetMaxHp(), QSize(108, 10));
                                qDebug()<< EnemyPokemon.GetCurrentHp() ;

                                t++;
                                if(t >= MyPokemon->GetSpecialatk(ButtonType).at(1)){
                                    timer->stop();
                                    timer->deleteLater();
                                    emit Attack_Dialog(0, ButtonType, true);
                                }

                            });
                            timer->start(1000);

                    });

                } else if (ButtonType == 3){
                    connect(button, &QToolButton::clicked, this, [=]() {
                        SpecialMenu->hide();
                        SkillMenu->show();
                    });
                } else{

                    button->setText("SA/DA :\n"+ QString::number(MyPokemon->GetSA()) + " / " + QString::number(MyPokemon->GetDA()));
                    button->setAttribute(Qt::WA_TransparentForMouseEvents);
                    button->setStyleSheet("QToolButton { background-color: white; color: black; font-size: 16pt; }");

                }

                break;
            }

            case 3:{ // Item
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
            case 4: { // Pokémon
                button = new QToolButton(PokemonMenu);
                button->setFocusPolicy(Qt::NoFocus);
                button->setIconSize(QSize(60, 60));
                button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                button->setStyleSheet("border: none; font-size: 18px;");

                if (ButtonType <= 3 && bag->Pokemon_List.size() >= ButtonType + 1 && bag->Pokemon_List.at(ButtonType).GetID() != -1) {
                    button->setIcon(QIcon(bag->Pokemon_List.at(ButtonType).GetImagePath()));
                    button->setText(QString::number(bag->Pokemon_List.at(ButtonType).GetCurrentHp()) + " / " + QString::number(bag->Pokemon_List.at(ButtonType).GetMaxHp()) + " Lv:" + QString::number(bag->Pokemon_List.at(ButtonType).GetLevel()));
                } else if (ButtonType <= 3) {
                    button->hide();
                }

                switch (ButtonType) {
                case 0: button->setGeometry(25, 20, 100, 100); break;
                case 1: button->setGeometry(130, 20, 100, 100); break;
                case 2: button->setGeometry(235, 20, 100, 100); break;
                case 3: button->setGeometry(340, 20, 100, 100); break;
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
    //int EnemyLevel = 1; //初始皆為1級
    qDebug() <<"創建EnemyID當掉";
    qDebug() <<"創建EnemyLevel當掉";
    int EnemyLevel = QRandomGenerator::global()->bounded((MyPokemon->GetLevel()-2 >= 1)? MyPokemon->GetLevel()-2 : 1, MyPokemon->GetLevel());
    qDebug() <<"創建EnemyLevel當掉";
    qDebug() << EnemyLevel ;
    PokemonData Enemy;
    if(EnemyLevel>5){
        PokemonData enemy(EnemyId, 5);
        for(int level = 5 ; level < EnemyLevel; level++){
            qDebug()<< level ;
            qDebug()<< EnemyLevel ;
            enemy.LevelUp();
        }
        Enemy = enemy;
    }else{

        PokemonData enemy(EnemyId, EnemyLevel);
        Enemy = enemy;
    }


    qDebug() << "🎯 生成敵人 ID:" << EnemyId << " Level:" << EnemyLevel;
    return Enemy;
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

void BattleScene::Attack_Dialog_slot(int Who,int MoveID, bool special) {

    CurrentDialogIndex = 0;
    PokemonData myPokemon = *MyPokemon;
    BattleScene::Who = Who;
    AttackDialogs ={
            {
             myPokemon.GetName() + " used " + ((special)? myPokemon.GetSpecialATK(MoveID):myPokemon.GetMove(MoveID) )+ " ! " ,
             EnemyPokemon.GetName() + " took " + ((special)? QString::number(myPokemon.GetDamage(EnemyPokemon,-1,MoveID)) + " x " +QString::number(myPokemon.GetSpecialatk(MoveID).at(1)) : QString::number(myPokemon.GetDamage(EnemyPokemon,MoveID))) + " damage!",
            "Enemy Turn"
            },
            {
            EnemyPokemon.GetName() + " used " +((special)? EnemyPokemon.GetSpecialATK(MoveID): EnemyPokemon.GetMove(MoveID)) + "!" ,
            myPokemon.GetName() + " took " + ((special)? QString::number(EnemyPokemon.GetDamage(myPokemon,-1,MoveID)) + " x " +QString::number(myPokemon.GetSpecialatk(MoveID).at(1)) : QString::number(EnemyPokemon.GetDamage(myPokemon,MoveID))) + " damage!",

            }
    };
    if (EnemyPokemon.GetCurrentHp() <= 0)
        AttackDialogs[0].push_back("You won!");
    if (MyPokemon->GetCurrentHp() > 0)
        AttackDialogs[1].push_back("Player Turn");

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
        Timer->start(2000);
    }


}
void BattleScene::Pokemon_Switch_Dialog_slot(int Seletion) {
    CurrentDialogIndex = 0;

    Pokemon_Switch_Dialogs ={//[Item][哪個item]
                     {"Switch to " + bag->Pokemon_List.at(Seletion).GetName(),
                             "Enemy Turn"},
//                     {"Used Potion!" + MyPokemon->GetName() +" restores 10 HP "  },
//                     {"Used Ether!" + MyPokemon->GetMove(RestoreMoveID) +" PP fully restored" }
                    };
    if (!Pokemon_Switch_Dialogs.isEmpty()) {
        Dialog->setText(Pokemon_Switch_Dialogs[0][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(3000);
    }
}

void BattleScene::Pokemon_Dead_Dialog_slot() {
    CurrentDialogIndex = 0;

    Pokemon_Dead_Dialogs ={
                     {MyPokemon->GetName()+" is dead.",
                             "Cherish UR Pokemon , idiot."},
//                     {"Used Potion!" + MyPokemon->GetName() +" restores 10 HP "  },
//                     {"Used Ether!" + MyPokemon->GetMove(RestoreMoveID) +" PP fully restored" }
                    };
    if (bag->GetNextAlivePokemonID() == -1)
        Pokemon_Dead_Dialogs[0].push_back("Loser , Fuck you.");
    if (!Pokemon_Dead_Dialogs.isEmpty()) {
        Dialog->setText(Pokemon_Dead_Dialogs[0][CurrentDialogIndex]);
        Dialog->show();
        Timer->start(3000);
    }
}


void BattleScene::ShowNextDialog() {
    CurrentDialogIndex++;

    if(!AttackDialogs.isEmpty()){
        if (CurrentDialogIndex < AttackDialogs[Who].size() ){
            if((AttackDialogs[Who][CurrentDialogIndex] == "Enemy Turn") && (EnemyPokemon.GetCurrentHp() <= 0)) CurrentDialogIndex++;
            Dialog->setText(AttackDialogs[Who][CurrentDialogIndex]);
            Timer->start(2000); // 繼續計時下一句
        }else {
            AttackDialogs.clear();
            Dialog->hide();
            if(MyPokemon->GetCurrentHp()<=0) Pokemon_Dead_Dialog_slot();
            else emit DialogFinished();

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
    }else if (!Pokemon_Dead_Dialogs.isEmpty()){
        if (CurrentDialogIndex < Pokemon_Dead_Dialogs[0].size()) {
            if((Pokemon_Dead_Dialogs[0][CurrentDialogIndex] == "Cherish UR Pokemon , idiot.")&& bag->GetNextAlivePokemonID() == -1) CurrentDialogIndex++;
            Dialog->setText(Pokemon_Dead_Dialogs[0][CurrentDialogIndex]);
            Timer->start(2000); // 繼續計時下一句
        } else {
            Pokemon_Dead_Dialogs.clear();
            Dialog->hide();
            Dead_And_SwitchToAnotherPokemon(); // 若有需要可加上
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
    int Random = QRandomGenerator::global()->bounded(0, 2);
    switch(Random){
    case 0:{
        int Enemy_move = QRandomGenerator::global()->bounded(0, 1);
         MyPokemon->TakeDamage(EnemyPokemon.GetDamage(myPokemon,Enemy_move));
         UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));

         emit Attack_Dialog(1,Enemy_move);

        break;
    }
    case 1:{
        int Special = QRandomGenerator::global()->bounded(0, (EnemyPokemon.GetLevel() <=2)? 1: (EnemyPokemon.GetLevel() <=4)? 2 : 3);
        int t = 0;
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() mutable {
            if(t < MyPokemon->GetSpecialatk(Special).at(1)){
            MyPokemon->TakeDamage(EnemyPokemon.GetDamage(myPokemon, -1 ,Special));
            UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
            qDebug()<< MyPokemon->GetCurrentHp() ;
            }
            t++;
            if(t >= MyPokemon->GetSpecialatk(Special).at(1)){
                timer->stop();
                timer->deleteLater();
                emit Attack_Dialog(1, Special, true);
            }

        });
        timer->start(1000);


        break;
    }
    case 2: break;
    }



}
void BattleScene::Player_turn(){
    MainMenu->show();
}


bool BattleScene::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Enter) {
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
void BattleScene::Dead_And_SwitchToAnotherPokemon() {
    int BagLocation = MyPokemon->GetBagLocation();

    bag->Remove_Pokemon(BagLocation);

    int nextId = bag->GetNextAlivePokemonID();
    qDebug() << "nexId is " << nextId;
    if (nextId == -1) {
        emit GameOver();


    } else {
        MyPokemon = GetPokemon_From_List(nextId);
        UpdateBattleInfo();
        UpdateHPBar(MyHpBarLabel,MyPokemon->GetCurrentHp(), MyPokemon->GetMaxHp(), QSize(108, 10));
        emit DialogFinished();

    }

}
