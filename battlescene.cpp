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
    connect(fightButton, &QPushButton::clicked, [=]() {
        qDebug() << "選擇 FIGHT";
    });
    connect(bagButton, &QPushButton::clicked, [=]() {
        qDebug() << "選擇 BAG";
    });
    connect(pokemonButton, &QPushButton::clicked, [=]() {
        qDebug() << "選擇 POKEMON";
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
    EnemyName->move(47, 61);
    EnemyName->setStyleSheet("font-weight: bold; font-size: 30px;");
    EnemyName->show();


    myPokemon = GetPokemon_From_List(0);

    QLabel* MyPokemonImage = new QLabel(this);
    QPixmap myImage(myPokemon.GetBackImagaePath());
    MyPokemonImage->setPixmap(myImage.scaled(150, 150, Qt::KeepAspectRatio));
    MyPokemonImage->move(62, 209);
    MyPokemonImage->show();

    QLabel* MyPokemonName = new QLabel(myPokemon.GetName(), this);
    MyPokemonName->move(311, 225);
    MyPokemonName->setStyleSheet("font-weight: bold; font-size: 30px;");
    MyPokemonName->show();



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

void BattleScene::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void BattleScene::ShowBattleMessage(const QString &msg) {
    Q_UNUSED(msg);
    // 未使用訊息框，保留未來可加入
}

PokemonData BattleScene::GenerateRandomEnemy() {
    int enemyId = QRandomGenerator::global()->bounded(0, 2); // 假設 0-2 是合法寶可夢ID
    int enemyForm = 0; // 初始等級皆為1(只有初始型態)

    PokemonData enemy(enemyId, enemyForm);
    qDebug() << "🎯 生成敵人 ID:" << enemyId << " Form:" << enemyForm;
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
