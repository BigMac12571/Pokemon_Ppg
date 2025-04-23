#include "dialog.h"
Dialog::Dialog(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(525, 99);
    this->move(0, View_Height - 99);

    Oak_dialog << "I am Professor Oak. Welcome to my laboratory!"
               << "You can choose one from three Poké Balls as your initial\nPokémon in Laboratory.";

    Sign_dialog <<"This is Pallet Town. Begin your adventure!";

    Grassland_dialog <<"In this place, there might be a wild Pokémon\nhiding in the tallgrass."
                     <<"Be careful!!!";

    Pickup_Pokeballs_dialog = {
            {
                "這是妙蛙種子（Bulbasaur）！",
                "它是草系的寶可夢，非常適合新手！",
                "Yes/No"
            },
            {
                "這是傑尼龜（Squirtle）！",
                "它是水系的寶可夢，冷靜又可靠！",
                "Yes/No"
            },
            {
                "這是小火龍（Charmander）！",
                "它是火系的寶可夢，有著燃燒的鬥志！",
                "Yes/No"
            }
        };

    Reset_Dialog_State();



    CurrentDialog = 0;


    setAlignment(Qt::AlignCenter);  // 文字居中顯示
    setStyleSheet("color: black; font-size: 20px;");


}
void Dialog::Reset_Dialog_State() {
    Oak_dialog_start = false;
    Sign_dialog_start = false;
    Pickup_Pokeballs_dialog_start = false;
    Waiting_For_YesNo = false;
    CurrentDialog = 0; // 重置对话索引
}

void Dialog::Oak_Dialog(){
    Oak_dialog_start = true;
    if (CurrentDialog < Oak_dialog.size()) {

        setText(Oak_dialog.at(CurrentDialog));
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        Reset_Dialog_State();
    }
}

void Dialog::Sign_Dialog(){
    Sign_dialog_start = true;
    if (CurrentDialog < Sign_dialog.size()) {

        setText(Sign_dialog.at(CurrentDialog));
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        Reset_Dialog_State();
    }
}


void Dialog::Grassland_Dialog(){
    Grassland_dialog_start = true;
    if (CurrentDialog < Grassland_dialog.size()) {

        setText(Grassland_dialog.at(CurrentDialog));
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        CurrentDialog = 0;
        Grassland_dialog_start = false;
    }
}



void Dialog::Show_Pokeballs_Dialog(int id){
    Pickup_Pokeballs_dialog_start = true;

    Shared_pokeball_ID = id;

    if (CurrentDialog < Pickup_Pokeballs_dialog[id].size() - 1) {
            setText(Pickup_Pokeballs_dialog[id].at(CurrentDialog));
            CurrentDialog++;

        } else if (CurrentDialog == Pickup_Pokeballs_dialog[id].size() - 1) {

            setText(Pickup_Pokeballs_dialog[id].at(CurrentDialog));
            CurrentDialog++;
            Waiting_For_YesNo = true;


        }
}







void Dialog::paintEvent(QPaintEvent *event)
{
    // 確保在 QLabel 顯示文字時仍然保持圖片顯示
    QPainter PaintDialog(this);
    QPixmap pixmap(":/new/prefix1/Dataset/Image/dialog.png");
    // 繪製背景圖片
    PaintDialog.drawPixmap(0, 0, width(), height(), pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
        else if(Sign_dialog_start) Sign_Dialog();

        else if(Pickup_Pokeballs_dialog_start && !Waiting_For_YesNo) Show_Pokeballs_Dialog(Shared_pokeball_ID);
        break;
    case Qt::Key_Y:
            if (Waiting_For_YesNo) {
                emit Pickup_Pokeballs(Shared_pokeball_ID);
                emit Close_Dialog();
                Reset_Dialog_State();
            }
            break;


    case Qt::Key_N:
            if (Waiting_For_YesNo) {
                emit Close_Dialog();
                Reset_Dialog_State();
            }
            break;
    }


}
