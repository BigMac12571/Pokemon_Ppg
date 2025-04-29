#include "battlescene.h"
#include <QDebug>
#include <QTimer>

BattleScene::BattleScene(Bag *myBag, QWidget *parent)
    : QWidget(parent), playerPokemon(nullptr), enemyPokemon(nullptr), bag(myBag) , isPlayerTurn(true),myPokemon(0,0),EnemyPokemon(0,0)
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
    });
    connect(pokemonButton, &QPushButton::clicked,this, [=]() {
        qDebug() << "選擇 POKEMON";
    });

    // 技能選單區
    SkillMenu = new QWidget(this);
    SkillMenu->setGeometry(0, 314, 525, 136); // 與原本按鈕區域重疊
    SkillMenu->hide(); // 一開始先隱藏
    QLabel* SkillMenuBackground = new QLabel(SkillMenu);
    SkillMenuBackground->setPixmap(QPixmap(":/your/image/path.png"));
    SkillMenuBackground->setGeometry(0, 0, 525, 136);
    SkillMenuBackground->raise();


    QPushButton* normalAttackButton = new QPushButton("Attack", SkillMenu);
    QPushButton* Power1Button = new QPushButton("Power 1", SkillMenu);
    QPushButton* Power2Button = new QPushButton("Power 2", SkillMenu);
    QPushButton* backButton = new QPushButton("Back", SkillMenu);

    normalAttackButton->setGeometry(15, 15, 100, 30);
    Power1Button->setGeometry(145, 15, 100, 30);
    Power2Button->setGeometry(15, 55, 100, 30);
    backButton->setGeometry(145, 55, 100, 30);

    connect(backButton, &QPushButton::clicked,this, [=]() {
        SkillMenu->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();
    });

    connect(normalAttackButton, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(myPokemon.GetDamage(EnemyPokemon,0)); // 普通攻擊傷害 10
        UpdateHPBar(enemyHpBarLabel,EnemyPokemon.GetCurrentHp(), myPokemon.GetMaxHp(), QSize(108, 10));

        SkillMenu->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();
    });

    connect(Power1Button, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(myPokemon.GetDamage(EnemyPokemon,1)); // 技能1 傷害 20
        UpdateHPBar(enemyHpBarLabel,EnemyPokemon.GetCurrentHp(), myPokemon.GetMaxHp(), QSize(108, 10));

        SkillMenu->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();
    });

    connect(Power2Button, &QPushButton::clicked,this, [=]() {
        EnemyPokemon.TakeDamage(myPokemon.GetDamage(EnemyPokemon,2)); // 技能2 傷害 30
        UpdateHPBar(enemyHpBarLabel,EnemyPokemon.GetCurrentHp(), myPokemon.GetMaxHp(), QSize(108, 10));

        SkillMenu->hide();
        fightButton->show();
        bagButton->show();
        pokemonButton->show();
        runButton->show();
    });
}

void BattleScene::StartBattle() {
    EnemyPokemon = GenerateRandomEnemy();

    QLabel* EnemyImage = new QLabel(this);
    QPixmap pix(EnemyPokemon.GetImagePath());
    EnemyImage->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio));
    EnemyImage->move(315, 54);
    EnemyImage->show();

    QLabel* EnemyName = new QLabel(EnemyPokemon.GetName(), this);
    EnemyName->move(47, 54);
    EnemyName->setStyleSheet("font-weight: bold; font-size: 25px;");
    EnemyName->show();

    QLabel* EnemyLevel = new QLabel("Lv:"+QString::number(EnemyPokemon.GetLevel()), this);
    EnemyLevel->move(170, 58);
    EnemyLevel->setStyleSheet("font-weight: bold; font-size: 25px;");
    EnemyLevel->show();

    enemyHpBarLabel = new QLabel(this);
    UpdateHPBar(enemyHpBarLabel,myPokemon.GetCurrentHp(), myPokemon.GetMaxHp(), QSize(108, 10));
    enemyHpBarLabel->move(112, 92); // 調整位置
    enemyHpBarLabel->show();

    myPokemon = GetPokemon_From_List(0);

    QLabel* MyPokemonImage = new QLabel(this);
    QPixmap myImage(myPokemon.GetBackImagaePath());
    MyPokemonImage->setPixmap(myImage.scaled(150, 150, Qt::KeepAspectRatio));
    MyPokemonImage->move(62, 209);
    MyPokemonImage->show();

    QLabel* MyPokemonName = new QLabel(myPokemon.GetName(), this);
    MyPokemonName->move(311, 225);
    MyPokemonName->setStyleSheet("font-weight: bold; font-size: 25px;");
    MyPokemonName->show();

    QLabel* MyLevel = new QLabel("Lv:"+QString::number(myPokemon.GetLevel()), this);
    MyLevel->move(443, 225);
    MyLevel->setStyleSheet("font-weight: bold; font-size: 25px;");
    MyLevel->show();

    myHpBarLabel = new QLabel(this);
    UpdateHPBar(myHpBarLabel,myPokemon.GetCurrentHp(), myPokemon.GetMaxHp(), QSize(108, 10));
    myHpBarLabel->move(381, 258);
    myHpBarLabel->show();


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
    // 空實作：未來可加入寶可夢資訊更新邏輯
}

void BattleScene::UseMove(int moveIndex) {
    Q_UNUSED(moveIndex);
}

void BattleScene::UseItem(int itemIndex) {
    Q_UNUSED(itemIndex);
}

void BattleScene::RunAway() {
    qDebug() << "逃跑成功！回到草地";

    // 模擬逃跑動畫延遲或提示
    QTimer::singleShot(500, [this]() {
        emit BattleEnd(false); // false = 沒有贏，是逃跑
    });
}
void BattleScene::BattleEnd(bool win) {
    if (win) {
        qDebug() << "You win!";
    } else {
        qDebug() << "You lose!";
    }
    // 你可以在這裡結束戰鬥畫面、切換場景、恢復狀態等
}


void BattleScene::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void BattleScene::ShowBattleMessage(const QString &msg) {
    Q_UNUSED(msg);
    // 未使用訊息框，保留未來可加入
}

PokemonData BattleScene::GenerateRandomEnemy() {
    int EnemyId = QRandomGenerator::global()->bounded(0, 2); // 假設 0~2 是合法寶可夢ID
    int EnemyLevel = 1; //初始皆為1級
    PokemonData enemy(EnemyId, EnemyLevel);
    qDebug() << "🎯 生成敵人 ID:" << EnemyId << " Level:" << EnemyLevel;
    return enemy;
}

PokemonData BattleScene::GetPokemon_From_List(int id)
{

    if (id >= 0 && id < bag->Pokemon_List.size())
    {
        qDebug() << "✅ Bag 裡有寶可夢：" << bag->Pokemon_List.at(id).GetName();
        return bag->Pokemon_List.at(id);
    }
    else
    {
        qDebug() << "❌ Invalid Pokemon id:" << id << " Bag 大小：" << bag->Pokemon_List.size();
        return PokemonData();
    }
}
