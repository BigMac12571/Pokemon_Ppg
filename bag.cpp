#include "bag.h"
#include "dialog.h"

Bag::Bag(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(296, 378);
    bag_image.load(":/new/prefix1/Dataset/Image/bag.png");
    this->move(115, 36);

    MenuContainer = new QWidget(this);
    MenuContainer->setGeometry(0, 0, width(), height()); // 讓它跟 Bag 一樣大

    // 然後再把 GridLayout 用在 MenuContainer 上
    Menu = new QGridLayout(MenuContainer);
    Menu->setSpacing(7*2);
    Menu->setContentsMargins(9*2, 47*2,70, 14);

    // 使用 MenuContainer 作為 layout
    MenuContainer->setLayout(Menu);

    open = false;

    for(int i = 0; i< 4 ; i++){
        Pokemon_image.append(new QLabel(this));
        Pokemon_name.append(new QLabel(this));
        Menu->addWidget(Pokemon_image.at(i), i, 0);
        Menu->addWidget(Pokemon_name.at(i), i, 1);
    }
}

Bag::~Bag() {
    delete pokeballLabel;
    delete potionLabel;
    delete etherLabel;
    //ClearBag();
}

void Bag::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!bag_image.isNull()) {
        QPixmap scaledBg = bag_image.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
         painter.drawPixmap(0, 0, scaledBg);
    } else {
        painter.fillRect(rect(), Qt::darkGray);
    }
    QWidget::paintEvent(event);
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

void Bag::Add_Pokemon(int id, int level) {
    PokemonData new_pokemon(id,level);
    Pokemon_List.append(new_pokemon);
    QPixmap image = new_pokemon.GetImagePath();
    Pokemon_image.at(id)->setPixmap(image.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    Pokemon_name.at(id)->setStyleSheet("color: black; font-size: 30px;");
    Pokemon_name.at(id)->setText(new_pokemon.GetName());

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
        if (id == 0 && pokeball == 3 && maxPokeballLabel == nullptr) {
                        maxPokeballLabel = new QLabel("(max)", this);
                        maxPokeballLabel->move(82, 56);
                        maxPokeballLabel->setStyleSheet("font-size: 16px; color: black; font-weight: bold;");
                        maxPokeballLabel->show();
                        maxPokeballLabel->raise();
                    } else if (id == 0 && pokeball < 3 && maxPokeballLabel != nullptr) {
                        delete maxPokeballLabel;
                        maxPokeballLabel = nullptr;
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

