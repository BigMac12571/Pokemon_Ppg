#include "battlescene.h"
#include <QDebug>
#include <QTimer>

BattleScene::BattleScene(QWidget *parent)
    : QWidget(parent), playerPokemon(nullptr), enemyPokemon(nullptr), bag(nullptr), isPlayerTurn(true)
{
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
    connect(fightButton, &QPushButton::clicked, [this]() {
        qDebug() << "選擇 FIGHT";
    });
    connect(bagButton, &QPushButton::clicked, [this]() {
        qDebug() << "選擇 BAG";
    });
    connect(pokemonButton, &QPushButton::clicked, [this]() {
        qDebug() << "選擇 POKEMON";
    });
}

void BattleScene::StartBattle(Bulbasaur* player, Bulbasaur* enemy) {
    playerPokemon = player;
    enemyPokemon = enemy;
    isPlayerTurn = true;
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
