#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(525,450);

    ///////////////////////新增stack/////////////////////////

    Scene_stack = new QStackedWidget(this);







    /////// 初始化每個畫面
    titlescreen = new TitleScreen(this);
    town = new Town(this);
    laboratary = new Laboratory(this);
    grassland = new Grassland(this);
    //battlescene = new BattleScene(this);
    /////// 初始化每個畫面

    /////// 初始化每個角色
    player = new Player(this);
    player->hide();


    /////// 初始化每個角色




    /////// 畫面加進 stack
    Scene_stack->addWidget(titlescreen);      // index 0
    Scene_stack->addWidget(town);       // index 1
    Scene_stack->addWidget(laboratary);   // index 2
    Scene_stack->addWidget(grassland);     // index 3
    //Scene_stack->addWidget(battlescene);      // index 4
    /////// 畫面加進 stack









    ///////////////////////新增stack/////////////////////////

    setCentralWidget(Scene_stack);
    Scene_stack->setCurrentIndex(0);// 預設顯示 Title 畫面
    switch_windowtitle(0);

    connect(titlescreen, &TitleScreen::Game_Start, this, [=]() {
        Scene_stack->setCurrentIndex(1); // 進入 Town 畫面
        switch_windowtitle(1); // 換window title

        town->Add_Player_To_Scene(player);
        town->SetMainPlayer(player); // <--- 指定主角
        //for (int i = 0; i < 4; ++i) {
        //    for (int j = 0; j < 3; ++j) {
        //        town->Add_Player_To_Scene(playerMatrix[i][j]);
        //        playerMatrix[i][j]->move(50 + j * 40, 100 + i * 60); // 位置間隔設定
        //    }
        //
    });
    connect(town, &Town::Enter_Laboratory, this, &MainWindow::Switch_TownToLaboratory);
    connect(laboratary, &Laboratory::Exit_Laboratory, this, &MainWindow::Switch_LaboratoryToTown);

    connect(town, &Town::Enter_Grassland, this, &MainWindow::Switch_TownToGrassland);
    connect(grassland, &Grassland::Exit_Grassland, this, &MainWindow::Switch_GrasslandToTown);










}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switch_windowtitle(int index) {
    Scene_stack->setCurrentIndex(index);

    switch (index) {
    case 0: this->setWindowTitle("Title Screen"); break;
    case 1: this->setWindowTitle("Town"); break;
    case 2: this->setWindowTitle("Laboratory"); break;
    case 3: this->setWindowTitle("Grassland"); break;
    case 4: this->setWindowTitle("Battle"); break;
    default: this->setWindowTitle("Scene"); break;
    }
}



void MainWindow::Switch_TownToLaboratory() {

    Scene_stack->setCurrentIndex(2); // 切到 laboratory 畫面
    switch_windowtitle(2);           // 更新視窗標題

    laboratary->Add_Player_To_Scene(player); // 將玩家加進新場景
    laboratary->SetMainPlayer(player);       // 更新主角控制權


}



void MainWindow::Switch_LaboratoryToTown() {


    Scene_stack->setCurrentIndex(1); // 回 Town
    switch_windowtitle(1);

    town->Add_Player_To_Scene(player);
    town->SetMainPlayer(player);

}


void MainWindow::Switch_TownToGrassland() {

    Scene_stack->setCurrentIndex(3); // 切到 laboratory 畫面
    switch_windowtitle(3);           // 更新視窗標題

    grassland->Add_Player_To_Scene(player); // 將玩家加進新場景
    grassland->SetMainPlayer(player);       // 更新主角控制權

}


void MainWindow::Switch_GrasslandToTown() {


    Scene_stack->setCurrentIndex(1); // 回 Town
    switch_windowtitle(1);

    town->Add_Player_To_Scene(player);
    town->SetMainPlayer(player);
    town->SetMainPlayer_GrasslandToTown(player);

}
