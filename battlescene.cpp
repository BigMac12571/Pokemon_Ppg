#include "battlescene.h"
#include <QDebug>

BattleScene::BattleScene(QWidget *parent)
    : QWidget(parent), playerPokemon(nullptr), enemyPokemon(nullptr), bag(nullptr), isPlayerTurn(true)
{
    SetupUI();
}

void BattleScene::SetupUI() {
    background = new QLabel(this);
    background->setPixmap(QPixmap(":/images/battle_scene.png"));
    background->setGeometry(0, 0, 525, 450);

    playerLabel = new QLabel("Player Pokemon", this);
    playerLabel->setGeometry(50, 250, 120, 30);

    enemyLabel = new QLabel("Enemy Pokemon", this);
    enemyLabel->setGeometry(350, 50, 120, 30);

    messageBox = new QLabel(this);
    messageBox->setGeometry(20, 360, 480, 60);
    messageBox->setStyleSheet("background-color: white; border: 1px solid black;");

    fightButton = new QPushButton("Fight", this);
    fightButton->setGeometry(400, 250, 100, 30);
    connect(fightButton, &QPushButton::clicked, [this]() {
        ShowBattleMessage("使用技能攻擊！");
    });

    runButton = new QPushButton("Run", this);
    runButton->setGeometry(400, 300, 100, 30);
    connect(runButton, &QPushButton::clicked, this, &BattleScene::RunAway);
}

void BattleScene::StartBattle(Bulbasaur* player, Bulbasaur* enemy) {
    playerPokemon = player;
    enemyPokemon = enemy;
    isPlayerTurn = true;
    UpdateBattleInfo();
}

void BattleScene::UpdateBattleInfo() {
    if (playerPokemon && enemyPokemon) {
        playerLabel->setText("Bulbasaur Lv." + QString::number(1));
        enemyLabel->setText("Wild Bulbasaur Lv." + QString::number(1));
        ShowBattleMessage("對戰開始！");
    }
}

void BattleScene::UseMove(int moveIndex) {
    Q_UNUSED(moveIndex);
    ShowBattleMessage("Bulbasaur 使用技能了！");
}

void BattleScene::UseItem(int itemIndex) {
    Q_UNUSED(itemIndex);
    ShowBattleMessage("使用了道具！");
}

void BattleScene::RunAway() {
    ShowBattleMessage("你逃跑了……");
    emit BattleEnd(false);
}

void BattleScene::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    // 可以在這裡畫血條、效果文字等
}

void BattleScene::ShowBattleMessage(const QString &msg) {
    messageBox->setText(msg);
}


