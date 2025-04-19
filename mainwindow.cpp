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
    Character_stack = new QStackedWidget(this);

    /////// 初始化每個畫面
    titlescreen = new TitleScreen(this);
    town = new Town(this);
    //laboratary = new Laboratory(this);
    //grassland = new GrassLand(this);
    //battlescene = new BattleScene(this);
    /////// 初始化每個畫面

    /////// 初始化每個角色
    for(int i=0;i<12;i++){
        player[i] = new Player(this);
    }




    /////// 初始化每個角色




    /////// 畫面加進 stack
    Scene_stack->addWidget(titlescreen);      // index 0
    Scene_stack->addWidget(town);       // index 1
    //Scene_stack->addWidget(laboratary);   // index 2
    //Scene_stack->addWidget(grassland);     // index 3
    //Scene_stack->addWidget(battlescene);      // index 4
    /////// 畫面加進 stack
    for(int i=0;i<12;i++){
        Character_stack->addWidget(player[i]);
    }




    /////// 畫面加進 stack





    ///////////////////////新增stack/////////////////////////

    setCentralWidget(Scene_stack);
    Scene_stack->setCurrentIndex(0);// 預設顯示 Title 畫面
    switch_windowtitle(0);

    connect(titlescreen, &TitleScreen::Game_Start, this, [=]() {
        Scene_stack->setCurrentIndex(1); // 進入 Town 畫面
        switch_windowtitle(1); // 換windotitle
        Character_stack->setCurrentIndex(0);
        town->Add_Player_To_Scene(player[0]);
    });









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



