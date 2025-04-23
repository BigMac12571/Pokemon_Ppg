#include "bag.h"

Bag::Bag(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(296, 378);
    bag_image = new QLabel(this);
    QPixmap pixmap(":/new/prefix1/Dataset/Image/bag.png");
    bag_image->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    this->move(115, 36);

    open = false;

}

void Bag::Open_bag(){
    if(!open){
        this->show();
        this->raise();
    }
    else {
        this->hide();
       }
    open = !open;
}



void Bag::Add_pokemon(const QPixmap& IconPixmap, const QString& name) {
    if (IconPixmap.isNull()) {
            qDebug() << "Warning: addPokemonIcon received a null QPixmap!";
            return;
        }

        QLabel* IconLabel = new QLabel(this); // 创建新的 QLabel，父对象是 Bag
        IconLabel->setPixmap(IconPixmap); // 设置接收到的缩放后的图片
        IconLabel->setFixedSize(IconPixmap.size());
        int Icon_x = 200 ; // 固定 X 坐标
        int Icon_y = 94 + Pokemon_List.size() * 70;
        IconLabel->move(Icon_x, Icon_y);


        QLabel* NameLabel = new QLabel(this); // 创建新的 QLabel 用于显示名字，父对象也是 Bag
        NameLabel->setText(name);
        int Name_x = 40;
        int Name_y = 102 + Pokemon_List.size() * 70;
        NameLabel->move(Name_x, Name_y);
        NameLabel->setStyleSheet("font-size: 32px; color: black;");


        Pokemon_List.append(IconLabel);
        IconLabel->show(); // 确保图标是可见的
        IconLabel->raise(); // 确保图标在背景图片之上
        NameLabel->show();
        NameLabel->raise();

}

