#include "bulbasaur.h"

Bulbasaur::Bulbasaur(QWidget* parent) : QWidget(parent), Current_form(First)
{
    FirstForm_Image = new QLabel(parent);
    SecondForm_Image = new QLabel(parent);
    ThirdForm_Image = new QLabel(parent);

    QPixmap bulbasaur(":/new/prefix1/Dataset/Image/battle/bulbasaur.png");
    FirstForm_Image->setPixmap(bulbasaur);
    FirstForm_Image->setFixedSize(110, 110);

    QPixmap ivysaur(":/new/prefix1/Dataset/Image/battle/ivysaur.png");
    SecondForm_Image->setPixmap(ivysaur);
    SecondForm_Image->setFixedSize(144, 157);

    QPixmap venusaur(":/new/prefix1/Dataset/Image/battle/venusaur.png");

    ThirdForm_Image->setPixmap(venusaur);
    ThirdForm_Image->setFixedSize(186, 182);

     //this->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    FirstForm_Image->hide();
    SecondForm_Image->hide();
    ThirdForm_Image->hide();


}
void Bulbasaur::ShowFormImage(Form form, int x, int y) { //用於開場選擇寶貝球時

    QLabel* target = GetFormImage(form);
    if (target) {
        target->move(x, y);
        target->show();
        target->raise();
    }
    Current_form = form;
}
void Bulbasaur::HideFormImage(Form form){
    QLabel* target = GetFormImage(form);
         if (target) {
             target->hide();
     }

}

QLabel* Bulbasaur::GetFormImage(Form form){ //回傳Label元件
    switch(form) {
    case First: return FirstForm_Image; break;
    case Second: return SecondForm_Image; break;
    case Third: return ThirdForm_Image; break;
     }

}
QPixmap Bulbasaur::GetIconPixmap(const QSize& iconSize) { //回傳圖片
    QLabel* Current_Image_Label = GetFormImage(Current_form);

    if (Current_Image_Label && Current_Image_Label->pixmap()) {
        return Current_Image_Label->pixmap()->scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    return QPixmap();
}
