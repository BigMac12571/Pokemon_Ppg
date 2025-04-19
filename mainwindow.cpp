#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(525,480);

    ///////////////////////新增stack/////////////////////////

    Scene_stack = new QStackedWidget(this);
    Character_stack = new QStackedWidget(this);


    QString imagePaths[4][3] = {
        {":/new/prefix1/Dataset/Image/player/player_B.png", ":/new/prefix1/Dataset/Image/player/player_BW1.png", ":/new/prefix1/Dataset/Image/player/player_BW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_F.png", ":/new/prefix1/Dataset/Image/player/player_FW1.png", ":/new/prefix1/Dataset/Image/player/player_FW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_L.png", ":/new/prefix1/Dataset/Image/player/player_LW1.png", ":/new/prefix1/Dataset/Image/player/player_LW2.png"},
        {":/new/prefix1/Dataset/Image/player/player_R.png", ":/new/prefix1/Dataset/Image/player/player_RW1.png", ":/new/prefix1/Dataset/Image/player/player_RW2.png"}
    };





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
        player[i]->hide();
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            playerMatrix[i][j] = new Player(nullptr); // 不指定 parent，避免提前顯示
            playerMatrix[i][j]->setImage(imagePaths[i][j]); // 設定圖片
            playerMatrix[i][j]->hide(); // 預設隱藏，之後加到畫面再 show
        }
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
       // Character_stack->setCurrentIndex(0);
        town->Add_Player_To_Scene(player[0]);


        town->SetMainPlayer(player[0]); // <--- 指定主角

            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 3; ++j) {
                    town->Add_Player_To_Scene(playerMatrix[i][j]);
                    playerMatrix[i][j]->move(50 + j * 40, 100 + i * 60);
                }
            }
        //for (int i = 0; i < 4; ++i) {
        //    for (int j = 0; j < 3; ++j) {
        //        town->Add_Player_To_Scene(playerMatrix[i][j]);
        //        playerMatrix[i][j]->move(50 + j * 40, 100 + i * 60); // 位置間隔設定
        //    }
        //}
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



