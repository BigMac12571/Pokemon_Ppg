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



    mybag = new Bag(this);
    mybag->hide();

    /////// 初始化每個畫面
    titlescreen = new TitleScreen(this);
    town = new Town(this);
    laboratory = new Laboratory(this);
    grassland = new Grassland(mybag,this);
    battlescene = new BattleScene(mybag,this);
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








    /////// 畫面加進 stack
    Scene_stack->addWidget(titlescreen);      // index 0
    Scene_stack->addWidget(town);       // index 1
    Scene_stack->addWidget(laboratory);   // index 2
    Scene_stack->addWidget(grassland);     // index 3
    Scene_stack->addWidget(battlescene);      // index 4
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

    connect(grassland, &Grassland::Battle, this, &MainWindow::Switch_GrasslandToBattle);//觸發戰鬥
    connect(battlescene,&BattleScene::BattleEnd, this , &MainWindow::Switch_BattelToGrassland);//返回草地

    connect(laboratory, &Laboratory::Open_Dialog_Oak, this, &MainWindow::Oak_Dialog); //對話框
    connect(town, &Town::Open_Dialog_Sign, this, &MainWindow::Sign_Dialog);
    connect(grassland, &Grassland::Open_Dialog_Grassland_Sign, this, &MainWindow::Grassland_Dialog);
    connect(town, &Town::Open_Dialog_Box, this, &MainWindow::Box_Dialog);
    connect(dialog, &Dialog::Close_Dialog, this , &MainWindow::Close_Dialog);

    connect(laboratory, &Laboratory::Show_Pokeballs, this, [=](int id){ //顯示寶可夢圖片與對話
        Show_Pokeballs_Dialog_slot(id);
    });

    connect(dialog, &Dialog::Pickup_Pokeballs, this , &MainWindow::Pickup_Pokeballs_slot); //按Y撿起
    connect(dialog, &Dialog::Get_pokeball, this, &MainWindow::Add_pokeball);//記錄獲得的pokeball
    connect(dialog, &Dialog::Get_potion, this, &MainWindow::Add_potion);//記錄獲得的potion
    connect(dialog, &Dialog::Get_ether, this, &MainWindow::Add_ether);//記錄獲得的ether
    connect(town, &Town::Refresh_bag, this , &MainWindow::Refresh_bag);//刷新背包
    connect(laboratory, &Laboratory::Refresh_bag, this , &MainWindow::Refresh_bag);//刷新背包
    connect(grassland, &Grassland::Refresh_bag, this , &MainWindow::Refresh_bag);//刷新背包

    connect(town, &Town::Open_Bag_Signal, this , &MainWindow::Open_Bag_slot); //按B打開背包
    connect(grassland, &Grassland::Open_Bag_Signal, this , &MainWindow::Open_Bag_slot); //按B打開背包
    connect(laboratory, &Laboratory::Open_Bag_Signal, this , &MainWindow::Open_Bag_slot); //按B打開背包


//    connect(battlescene, &BattleScene::Attack_Dialog, this , &MainWindow::Attack_Dialog_slot);
//    connect(battlescene, &BattleScene::Items_Dialog, this , &MainWindow::Items_Dialog_slot);



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
    grassland->SetMainPlayer(player);// 更新主角控制權
}


void MainWindow::Switch_GrasslandToTown() {


    Scene_stack->setCurrentIndex(1); // 回 Town
    switch_windowtitle(1);

    town->Add_Player_To_Scene(player);
    town->SetMainPlayer(player);
    town->SetMainPlayer_GrasslandToTown(player);

}

void MainWindow::Switch_GrasslandToBattle(){
    //要觸發戰鬥至少要有一隻寶可夢
    if(!mybag->Pokemon_List.isEmpty()){
    Scene_stack->setCurrentIndex(4); // 進入battle
    switch_windowtitle(4);
    battlescene->StartBattle();





    }
}

void MainWindow::Switch_BattelToGrassland(bool win){
    if(win){
    }else{

    }

    Scene_stack->setCurrentIndex(3);
    switch_windowtitle(3);

    grassland->Add_Player_To_Scene(player); // 將玩家加進新場景
    grassland->SetMainPlayer(player);// 更新主角控制權
    grassland->SetLastPosition();




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

void MainWindow::Box_Dialog(){
    //QPoint pos = dialog->pos();
    //qDebug() << "Dialog position: " << pos;

    dialog->show();
    dialog->setFocus();
    dialog->Box_Dialog();
}

//void MainWindow::Attack_Dialog_slot(PokemonData *pokemon,PokemonData enemy_pokemon, int MoveID){

//    dialog->show();
//    dialog->setFocus();
//    dialog->Battle_Dialog(pokemon,enemy_pokemon,0,0,MoveID); // 0,0,MoveiD = attack對話，我，哪個move
//}
//void MainWindow::Items_Dialog_slot(PokemonData *pokemon,PokemonData enemy_pokemon, int ItemID){

//    dialog->show();
//    dialog->setFocus();
//    dialog->Battle_Dialog(pokemon,enemy_pokemon,1,ItemID,0); // 0,ItemID,本來是存moveid但這邊隨便放 = item對話，哪個item，哪個move(不需要)
//}


void MainWindow::Close_Dialog(){


    dialog->Reset_Dialog_State();
    dialog->close();
    player->stopWalking();



    town->clearPressedKeys();
    laboratory->clearPressedKeys();
    grassland->clearPressedKeys();

}


///////////////////實驗室拿pokeball
void MainWindow::Show_Pokeballs_Dialog_slot(int id){
    dialog->Show_Pokeballs_Dialog(id);
    dialog->Show_Pokemon(id);
    dialog->show();
    dialog->setFocus();

}
void MainWindow::Pickup_Pokeballs_slot(int id){

    if(id==0) {
        laboratory->Pokeball_get_picked(pokeball0);
        mybag->Add_Pokemon(id,1);
    }
    else if(id==1){
        laboratory->Pokeball_get_picked(pokeball1);
        mybag->Add_Pokemon(id,1);
    }
    else if(id==2) {
        laboratory->Pokeball_get_picked(pokeball2);
        mybag->Add_Pokemon(id,1);
    }
    qDebug() << "Bag 裡有" << mybag->Pokemon_List.size() << "隻";
}

void MainWindow::Add_pokeball(){
    mybag->Add_pokeball();
}

void MainWindow::Add_potion(){
    mybag->Add_potion();
}

void MainWindow::Add_ether(){
    mybag->Add_ether();
}

void MainWindow::Refresh_bag(){
    mybag->Refresh_bag(0);
    mybag->Refresh_bag(1);
    mybag->Refresh_bag(2);
}
///////////////////實驗室拿pokeball

///////////////////背包

void MainWindow::Open_Bag_slot(){
    mybag->Open_bag();

}
