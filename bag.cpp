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
    if(pokeball < 3){
        pokeball++;
        //qDebug() << "pokeball 的值現在是：" << pokeball;
    }
    Refresh_bag(0);
    //qDebug() << "Add_pokeball() 執行後，Refresh_bag(0) 被呼叫";
}

void Bag::Add_potion(){
    potion++;
    Refresh_bag(1);
}
void Bag::Add_ether(){
    ether++;
    Refresh_bag(2);
}
void Bag::Refresh_bag(int id) {
    //qDebug() << "Bag::Refresh_bag(" << id << ") 被呼叫，pokeball 的值是：" << pokeball;
    QString res = "x";
    int x, y;

    QLabel* currentLabel = nullptr;

    switch (id) {
    case 0:
        res = res + QString::number(pokeball);
        x = 92;
        y = 40;
        currentLabel = pokeballLabel;
        break;
    case 1:
        res += QString::number(potion);
        x = 176;
        y = 40;
        currentLabel = potionLabel;
        break;
    case 2:
        res += QString::number(ether);
        x = 272;
        y = 40;
        currentLabel = etherLabel;
        break;

    }

    if (currentLabel) {
        // 如果標籤已經存在，則更新文字
        //qDebug() << "更新 pokeballLabel: " << currentLabel << "，設定文字為：" << res;
        if(pokeball == 3 && id == 0){
            QLabel* NameLabel = new QLabel(this);
            NameLabel->setText("(max)");
            NameLabel->move(82, 56);
            NameLabel->setStyleSheet("font-size: 20px; color: black; font-weight: bold;");
            NameLabel->show();
            NameLabel->raise();
        }
        currentLabel->setText(res);
        currentLabel->setStyleSheet("font-size: 20px; color: black; font-weight: bold;");

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

