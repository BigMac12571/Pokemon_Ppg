#include "dialog.h"
Dialog::Dialog(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(View_Width, View_Height);
    this->move(0,0);
    Text = new QLabel(this);
    Text->setAlignment(Qt::AlignCenter);  // 文字居中顯示
    Text->setStyleSheet("color: black; font-size: 20px;");
    Text->setGeometry(0, View_Height-99, 525, 99);


    Oak_dialog << "I am Professor Oak. Welcome to my laboratory!"
               << "You can choose one from three Poké Balls as your initial\nPokémon in Laboratory.";

    Sign_dialog <<"This is Pallet Town. Begin your adventure!";

    Grassland_dialog <<"In this place, there might be a wild Pokémon\nhiding in the tallgrass."
                     <<"Be careful!!!";

    Box_dialog = {
        {
            "You get a Poké Ball!"
        },
        {
            "You get a Potion!"
        },
        {
            "You get an Ether!"
        }
    };

    Pickup_Pokeballs_dialog = {
            {
                "This is Bulbasaur!",
                "It's a grass type Pokémon and very suitable for beginners!",
                "    Yes    /    No    "
            },
            {
                "This is Squirtle!",
                "It's a water type Pokémon. Cool and Reliable!",
                "    Yes    /    No    "
            },
            {
                "This is Charmander!",
                "It's a fire type Pokémon, with burning fighting spirit!",
                "    Yes    /    No    "
            }
        };

    Reset_Dialog_State();



    CurrentDialog = 0;



}

void Dialog::Reset_Dialog_State() {
    Oak_dialog_start = false;
    Sign_dialog_start = false;
    Grassland_dialog_start = false;
    Box_dialog_start = false;
    Pickup_Pokeballs_dialog_start = false;
    Waiting_For_YesNo = false;
    CurrentDialog = 0; // 重置对话索引
}

void Dialog::Oak_Dialog(){
    Oak_dialog_start = true;
    if (CurrentDialog < Oak_dialog.size()) {

        Text->setText(Oak_dialog.at(CurrentDialog));
        Text->show();
        Text->raise();
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        Reset_Dialog_State();
    }
}

void Dialog::Sign_Dialog(){
    Sign_dialog_start = true;
    if (CurrentDialog < Sign_dialog.size()) {

        Text->setText(Sign_dialog.at(CurrentDialog));
        Text->show();
        Text->raise();
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        Reset_Dialog_State();
    }
}


void Dialog::Grassland_Dialog(){
    Grassland_dialog_start = true;
    if (CurrentDialog < Grassland_dialog.size()) {

        Text->setText(Grassland_dialog.at(CurrentDialog));
        Text->show();
        Text->raise();
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        Reset_Dialog_State();
    }
}

void Dialog::Box_Dialog(){
    Box_dialog_start = true;

    int id = QRandomGenerator::global()->generate() % 3;
    if (CurrentDialog < Box_dialog[id].size()) {

        Text->setText(Box_dialog[id].at(CurrentDialog));
        Text->show();
        Text->raise();
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        Reset_Dialog_State();
    }
    switch(id){
    case 0:
        emit Get_pokeball();
        break;
    case 1:
        emit Get_potion();
        break;
    case 2:
        emit Get_ether();
        break;
    }

}



void Dialog::Show_Pokeballs_Dialog(int id){
    Pickup_Pokeballs_dialog_start = true;

    Shared_pokeball_ID = id;

    if (CurrentDialog < Pickup_Pokeballs_dialog[id].size() - 1) {
            Text->setText(Pickup_Pokeballs_dialog[id].at(CurrentDialog));
            Text->show();
            Text->raise();

            CurrentDialog++;

        } else if (CurrentDialog == Pickup_Pokeballs_dialog[id].size() - 1) {

            Text->setText(Pickup_Pokeballs_dialog[id].at(CurrentDialog));
            Text->show();
            Text->raise();

            CurrentDialog++;
            Waiting_For_YesNo = true;


        }
}

void Dialog::Show_Pokemon(int id){
    Pokemon = new QLabel(this);
    QPixmap pokemon_image;
    switch(id){
    case 0:{pokemon_image.load(":/new/prefix1/Dataset/Image/battle/bulbasaur.png"); break;}
    case 1:{pokemon_image.load(":/new/prefix1/Dataset/Image/battle/squirtle.png"); break;}
    case 2:{pokemon_image.load(":/new/prefix1/Dataset/Image/battle/charmander.png"); break;}
    }
    Pokemon->setPixmap(pokemon_image);
    Pokemon->move( width()/2-110/2, height()/2-80);
    Pokemon->show();
    Pokemon->raise();
}





void Dialog::paintEvent(QPaintEvent *event)
{
    // 確保在 QLabel 顯示文字時仍然保持圖片顯示
    QPainter PaintDialog(this);
    QPixmap pixmap(":/new/prefix1/Dataset/Image/dialog.png");
    // 繪製背景圖片
    PaintDialog.drawPixmap(0, View_Height-99, 525, 99, pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel::paintEvent(event); // 顯示你設定的文字（像你用 setText() 設定的句子）

}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    switch(key){
    case Qt::Key_A:
        if(Oak_dialog_start) Oak_Dialog();
        else if(Sign_dialog_start) Sign_Dialog();
        else if(Grassland_dialog_start) Grassland_Dialog();
        else if(Box_dialog_start) {
                    emit Close_Dialog();
                    Reset_Dialog_State();
                }
        else if(Pickup_Pokeballs_dialog_start && !Waiting_For_YesNo){
            Show_Pokeballs_Dialog(Shared_pokeball_ID);
        }
        break;
    case Qt::Key_Y:
            if (Waiting_For_YesNo) {
                emit Pickup_Pokeballs(Shared_pokeball_ID);
                emit Close_Dialog();
                Reset_Dialog_State();
                delete Pokemon;
            }
            break;


    case Qt::Key_N:
            if (Waiting_For_YesNo) {
                emit Close_Dialog();
                Reset_Dialog_State();
                delete Pokemon;
            }
            break;
    }


}
