#include "charmander.h"

Charmander::Charmander(QWidget* parent) : QWidget(parent), Current_form(First)
{
    FirstForm_Image = new QLabel(parent);
    SecondForm_Image = new QLabel(parent);
    ThirdForm_Image = new QLabel(parent);

    QPixmap charmander(":/new/prefix1/Dataset/Image/battle/charmander.png");
    FirstForm_Image->setPixmap(charmander);
    FirstForm_Image->setFixedSize(110, 110);

    QPixmap charmeleon  (":/new/prefix1/Dataset/Image/battle/charmeleon.png");
    SecondForm_Image->setPixmap(charmeleon);
    SecondForm_Image->setFixedSize(144, 157);

    QPixmap charizard(":/new/prefix1/Dataset/Image/battle/charizard.png");

    ThirdForm_Image->setPixmap(charizard);
    ThirdForm_Image->setFixedSize(186, 182);

     //this->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    FirstForm_Image->hide();
    SecondForm_Image->hide();
    ThirdForm_Image->hide();


}
void Charmander::ShowFormImage(Form form, int x, int y) {

    QLabel* target = nullptr;
    switch(form) {
        case First: target = FirstForm_Image; break;
        case Second: target = SecondForm_Image; break;
        case Third: target = ThirdForm_Image; break;
     }
    qDebug() << "Bulbasaur widget size:" << this->size();
    qDebug() << "Target image size:" << target->size();
    qDebug() << "Target image pos:" << target->pos();


    if (target) {
        target->move(x, y);
        target->show();
        target->raise();
    }
}
void Charmander::HideFormImage(Form form){
    switch(form) {
    case First: FirstForm_Image->hide(); break;
    case Second: SecondForm_Image->hide(); break;
    case Third: ThirdForm_Image->hide(); break;
     }

}

