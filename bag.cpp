#include "bag.h"
#include "dialog.h"

Bag::Bag(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(296, 378);
    bag_image = new QLabel(this);
    QPixmap pixmap(":/new/prefix1/Dataset/Image/bag.png");
    bag_image->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    this->move(115, 36);

    open = false;


}

Bag::~Bag() {
    delete pokeballLabel;
    delete potionLabel;
    delete etherLabel;
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
void Bag::Add_pokeball(){
    pokeball++;
}
void Bag::Add_potion(){
    potion++;
}
void Bag::Add_ether(){
    ether++;
}
void Bag::Refresh_bag(int id) {
    QString res = "x";
    int x, y;

    QLabel* currentLabel = nullptr;

    switch (id) {
    case 0:
        res += QString::number(pokeball);
        x = 92;
        y = 52;
        currentLabel = pokeballLabel;
        break;
    case 1:
        res += QString::number(potion);
        x = 176;
        y = 52;
        currentLabel = potionLabel;
        break;
    case 2:
        res += QString::number(ether);
        x = 272;
        y = 52;
        currentLabel = etherLabel;
        break;
    default:
        return;
    }

    if (currentLabel) {
        // 如果標籤已經存在，則更新文字
        currentLabel->setText(res);
    } else {
        // 如果標籤不存在，則創建新的標籤並儲存指標
        QLabel* NameLabel = new QLabel(this);
        NameLabel->setText(res);
        NameLabel->move(x, y);
        NameLabel->setStyleSheet("font-size: 20px; color: black; font-weight: bold;");
        NameLabel->show();
        NameLabel->raise();

        switch (id) {
        case 0:
            pokeballLabel = NameLabel;
            break;
        case 1:
            potionLabel = NameLabel;
            break;
        case 2:
            etherLabel = NameLabel;
            break;
        }
    }
}

