#include "squirtle.h"

Squirtle::Squirtle(QWidget* parent) : QWidget(parent), Current_form(First)
{
    FirstForm_Image = new QLabel(parent);
    SecondForm_Image = new QLabel(parent);
    ThirdForm_Image = new QLabel(parent);

    QPixmap squirtle(":/new/prefix1/Dataset/Image/battle/squirtle.png");
    FirstForm_Image->setPixmap(squirtle);
    FirstForm_Image->setFixedSize(110, 110);

    QPixmap wartortle (":/new/prefix1/Dataset/Image/battle/ivysaur.png");
    SecondForm_Image->setPixmap(wartortle);
    SecondForm_Image->setFixedSize(144, 157);

    QPixmap blastoise(":/new/prefix1/Dataset/Image/battle/venusaur.png");

    ThirdForm_Image->setPixmap(blastoise);
    ThirdForm_Image->setFixedSize(186, 182);

     //this->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    FirstForm_Image->hide();
    SecondForm_Image->hide();
    ThirdForm_Image->hide();


}
void Squirtle::ShowFormImage(Form form, int x, int y) {

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
void Squirtle::HideFormImage(Form form){
    switch(form) {
    case First: FirstForm_Image->hide(); break;
    case Second: SecondForm_Image->hide(); break;
    case Third: ThirdForm_Image->hide(); break;
     }

}

