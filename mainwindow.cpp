#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(525,450); // 主視窗

    ///////////////////////新增stack/////////////////////////

    Scene_stack = new QStackedWidget(this);





    /////// 初始化每個畫面
    titlescreen = new TitleScreen(this);
    town = new Town(this);
    laboratory = new Laboratory(this);
    grassland = new Grassland(this);
    //battlescene = new BattleScene(this);
    /////// 初始化每個畫面

    /////// 初始化每個角色
    player = new Player(this); //主視窗角色指標
    player->hide();

    npc = new NPC(this); //npc
    npc->hide();
    /////// 初始化每個角色

    dialog = new Dialog(this); //對話
    dialog->hide();
    /////// 初始化寶貝球
    pokeball0 = new Pokeball(0,this);
    pokeball0->hide();

    pokeball1 = new Pokeball(1,this);
    pokeball1->hide();

    pokeball2 = new Pokeball(2,this);
    pokeball2->hide();
    /////// 初始化寶貝球


    /////// 初始化寶可夢
    bulbasaur = new Bulbasaur(this);
    squirtle = new Squirtle(this);
    charmander = new Charmander(this);



    /////// 初始化寶可夢

    bag = new Bag(this);
    bag->hide();






    /////// 畫面加進 stack
    Scene_stack->addWidget(titlescreen);      // index 0
    Scene_stack->addWidget(town);       // index 1
    Scene_stack->addWidget(laboratory);   // index 2
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

    });
    connect(town, &Town::Enter_Laboratory, this, &MainWindow::Switch_TownToLaboratory); //進出lab
    connect(laboratory, &Laboratory::Exit_Laboratory, this, &MainWindow::Switch_LaboratoryToTown);

    connect(town, &Town::Enter_Grassland, this, &MainWindow::Switch_TownToGrassland); //進出grassland
    connect(grassland, &Grassland::Exit_Grassland, this, &MainWindow::Switch_GrasslandToTown);

    connect(laboratory, &Laboratory::Open_Dialog_Oak, this, &MainWindow::Oak_Dialog); //對話框
    connect(town, &Town::Open_Dialog_Sign, this, &MainWindow::Sign_Dialog);
    connect(grassland, &Grassland::Open_Dialog_Grassland_Sign, this, &MainWindow::Grassland_Dialog);
    connect(dialog, &Dialog::Close_Dialog, this , &MainWindow::Close_Dialog);

    connect(laboratory, &Laboratory::Show_Pokeballs, this, [=](int id){ //顯示寶可夢圖片
        Show_Pokeballs_slot(id);
        Show_Pokeballs_Dialog_slot(id);
    });
    connect(dialog, &Dialog::Pickup_Pokeballs, this , &MainWindow::Pickup_Pokeballs_slot); //按Y撿起

    connect(town, &Town::Open_Bag_Signal, this , &MainWindow::Open_Bag_slot); //按B打開背包
    connect(grassland, &Grassland::Open_Bag_Signal, this , &MainWindow::Open_Bag_slot); //按B打開背包
    connect(laboratory, &Laboratory::Open_Bag_Signal, this , &MainWindow::Open_Bag_slot); //按B打開背包






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

    laboratory->Add_Player_To_Scene(player); // 將玩家加進新場景
    laboratory->SetMainPlayer(player);       // 更新主角控制權
    laboratory->Add_NPC_To_Scene(npc);
    laboratory->Add_Pokeball_To_Scene(0,pokeball0);
    laboratory->Add_Pokeball_To_Scene(1,pokeball1);
    laboratory->Add_Pokeball_To_Scene(2,pokeball2);

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
void MainWindow::Oak_Dialog(){
    //QPoint pos = dialog->pos();
    //qDebug() << "Dialog position: " << pos;

    dialog->show();
    dialog->setFocus();
    dialog->Oak_Dialog();
}
void MainWindow::Sign_Dialog(){
    //QPoint pos = dialog->pos();
    //qDebug() << "Dialog position: " << pos;

    dialog->show();
    dialog->setFocus();
    dialog->Sign_Dialog();
}
void MainWindow::Grassland_Dialog(){
    //QPoint pos = dialog->pos();
    //qDebug() << "Dialog position: " << pos;

    dialog->show();
    dialog->setFocus();
    dialog->Grassland_Dialog();
}




void MainWindow::Close_Dialog(){
    dialog->close();
    player->stopWalking();



    bulbasaur->HideFormImage(Bulbasaur::First);
    squirtle->HideFormImage(Squirtle::First);
    charmander->HideFormImage(Charmander::First);


}


///////////////////實驗室拿pokeball
void MainWindow::Show_Pokeballs_slot(int id){
    if(id==0) {
        bulbasaur->ShowFormImage(Bulbasaur::First, width()/2-110/2, height()/2-80);
    }
    else if(id==1){
        squirtle->ShowFormImage(Squirtle::First, width()/2-110/2, height()/2-80);
    }
    else if(id==2){
        charmander->ShowFormImage(Charmander::First, width()/2-110/2, height()/2-80);
    }



}
void MainWindow::Show_Pokeballs_Dialog_slot(int id){
    if(id==0) dialog->Show_Pokeballs_Dialog(id);
    else if(id==1) dialog->Show_Pokeballs_Dialog(id);
    else if(id==2) dialog->Show_Pokeballs_Dialog(id);

    dialog->show();
    dialog->setFocus();
}
void MainWindow::Pickup_Pokeballs_slot(int id){

    if(id==0) {laboratory->Pokeball_get_picked(pokeball0); bulbasaur->HideFormImage(Bulbasaur::First);}
    else if(id==1){ laboratory->Pokeball_get_picked(pokeball1); squirtle->HideFormImage(Squirtle::First);}
    else if(id==2) {laboratory->Pokeball_get_picked(pokeball2);charmander->HideFormImage(Charmander::First);}
}
///////////////////實驗室拿pokeball

///////////////////背包

void MainWindow::Open_Bag_slot(){
    bag->Open_bag();

}
