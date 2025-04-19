#include "titlescreen.h"

TitleScreen::TitleScreen(QWidget *parent)
    : QWidget(parent)
{
    ///////////////////////設定大小/////////////////////////
    this->setFixedSize(525,450);
    ///////////////////////設定大小/////////////////////////

    ///////////////////////設定背景/////////////////////////

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/start_menu.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(0, 0, width(), height()); // 填滿整個視窗
    ///////////////////////設定背景/////////////////////////



    ///////////////////////設定按鈕/////////////////////////

    press_start = new QPushButton("Press Start", this);
    press_start->setGeometry(85, 360, 200 ,50 );
    press_start->setStyleSheet(
        "QPushButton {"
        "  font-size: 32px;"
        "  font-family: 'Arial';"
        "  color: white;"
        "  background-color: rgba(0, 0, 0, 225);"

        "}"
        "QPushButton:hover {" //給滑鼠懸殊在上面時用的
        "  background-color: rgba(255, 255, 255, 0);"
        "}"
    );

    ///////////////////////設定按鈕/////////////////////////


    ///////////////////////閃爍效果/////////////////////////設定 opacity 效果
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(press_start);
    press_start->setGraphicsEffect(effect);
    effect->setOpacity (1.0);
    // Timer 控制透明度變化
    QTimer *flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, [=]() {
        static bool visible = true;
        if(visible){
            effect->setOpacity (0.0); // visible => 透明
            flashTimer->start(700);
        }else{
            effect->setOpacity (1.0); // not visible => 不透明
            flashTimer->start(2000);
        }
        visible = !visible;
    });

    flashTimer->start(2000);
    ///////////////////////閃爍效果/////////////////////////





    connect(press_start, &QPushButton::clicked, this, [=]() {
        emit Game_Start();  // ✅ 發出 signal
    });
}

