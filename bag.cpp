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
    Menu->setHorizontalSpacing(14);
    Menu->setVerticalSpacing(12);
    Menu->setContentsMargins(16,94 ,16, 14);
    //Menu->setContentsMargins(0,0 ,0, 0);
    // 使用 MenuContainer 作為 layout
    MenuContainer->setLayout(Menu);

    open = false;

    // 創建 4x3 的透明QLabel
    for (int row = 0; row < 4; ++row) {
        QList<QWidget*> rowPlaceholders;
        QList<QLabel*> rowImageLabels;
        QList<QLabel*> rowNameLabels;
        QList<QLabel*> rowLevelLabels;
        for (int col = 0; col < 3; ++col) {
            QWidget* placeholder = new QWidget(this);
            placeholder->setStyleSheet("background: transparent;");
            Menu->addWidget(placeholder, row, col);
            rowPlaceholders.append(placeholder);
            rowImageLabels.append(nullptr);
            rowNameLabels.append(nullptr);
            rowLevelLabels.append(nullptr);
        }
        pokemonPlaceholders.append(rowPlaceholders);
        ImageLabel.append(rowImageLabels);
        NameLabel.append(rowNameLabels);
        LevelLabel.append(rowLevelLabels);
    }


    //    for(int i = 0; i< 4 ; i++){
    //        Pokemon_image.append(new QLabel(this));
    //        Pokemon_name.append(new QLabel(this));
    //        Menu->addWidget(Pokemon_image.at(i), i, 0);
    //        Menu->addWidget(Pokemon_name.at(i), i, 1);
    //    }
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
//void Bag::Capture_Pokemon(PokemonData *NewPokemon) {
//    Add_Pokemon(NewPokemon->GetID(), NewPokemon->GetLevel());
//}

void Bag::Add_Pokemon(PokemonData NewPokemon) {

    int emptyIndex = -1;


    for (int i = 0; i < Pokemon_List.size(); ++i) {
        if (Pokemon_List[i].GetID() == -1) {
            emptyIndex = i;
            break;
        }
    }

    if (emptyIndex != -1) {
        Pokemon_List[emptyIndex] = NewPokemon;
        Pokemon_List[emptyIndex].SetBagLocation(emptyIndex);

    } else {
        Pokemon_List.append(NewPokemon);

        if (!Pokemon_List.isEmpty()) {
            Pokemon_List.last().SetBagLocation(Pokemon_List.size() - 1);
        } else {
        }
    }
    Refresh_bag_pokemon();
}

void Bag::Remove_Pokemon(int id) {
    if (id >= 0 && id < Pokemon_List.size()) {
            qDebug() << "Removing Pokémon at index:" << id
                     << " (ID:" << Pokemon_List[id].GetID()
                     << ", Name:" << Pokemon_List[id].GetName() << ")";
            Pokemon_List[id] = PokemonData();  // 清空成空的 slot
            //Refresh_bag_pokemon();
        } else {
            qDebug() << "Remove_Pokemon: Invalid index" << id;
        }
}

int Bag::GetNextAlivePokemonID() {
    for (int i = 0; i < Pokemon_List.size(); ++i) {
        if (Pokemon_List[i].GetCurrentHp() > 0 && Pokemon_List[i].GetID() != -1 )
            return i;
    }
    return -1; // 代表沒有存活的
}
void Bag::UsePokeball(){
    if(pokeball>0){

        pokeball --;
    }

    Refresh_bag(0);

}
void Bag::UsePotion(){
    if(potion>0){

        potion --;
    }

    Refresh_bag(1);

}
void Bag::UseEther(){
    if(ether>0){

        ether --;
    }

    Refresh_bag(2);

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
        x = 172;
        y = 40;
        currentLabel = potionLabel;
        break;
    case 2:
        res += QString::number(ether);
        x = 264;
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
        currentLabel->adjustSize();

    } else {
        // 如果標籤不存在，則創建新的標籤並儲存指標
        QLabel* NameLabel = new QLabel(this);
        NameLabel->setText(res);
        NameLabel->move(x, y);
        NameLabel->setStyleSheet("font-size: 20px; color: black; font-weight: bold;");
        NameLabel->adjustSize();
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

void Bag::Refresh_bag_pokemon() {
    for (auto& row : bag) {
        if (row) {
            delete row;
        }
    }
    bag.clear();

    for (int BagLocation = 0; BagLocation < 4; BagLocation++) {
        QToolButton* bag_a_space = new QToolButton(this);
        bag_a_space->setFocusPolicy(Qt::NoFocus);
        bag_a_space->setIconSize(QSize(60, 60));
        bag_a_space->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        bag_a_space->setAttribute(Qt::WA_TransparentForMouseEvents);
        bag_a_space->setStyleSheet("border: none; font-size: 20px;");
        if (Pokemon_List.size() > BagLocation && Pokemon_List.at(BagLocation).GetID() != -1) {
            bag_a_space->setIcon(QIcon(Pokemon_List.at(BagLocation).GetImagePath()));
            bag_a_space->setText(Pokemon_List.at(BagLocation).GetName()+" " +QString::number(Pokemon_List.at(BagLocation).GetCurrentHp()) + " / " +
                                 QString::number(Pokemon_List.at(BagLocation).GetMaxHp()) +
                                 " Lv:" + QString::number(Pokemon_List.at(BagLocation).GetLevel()));
            bag_a_space->show();  // 顯示
        } else {
            bag_a_space->hide();  // 隱藏
        }

        // 設定幾何位置
        switch (BagLocation) {
        case 0: bag_a_space->setGeometry(20, 96, 300, 60); break;
        case 1: bag_a_space->setGeometry(20, 164, 300, 60); break;
        case 2: bag_a_space->setGeometry(20, 236, 300, 60); break;
        case 3: bag_a_space->setGeometry(20, 310, 300, 60); break;
        default: break;
        }

        bag.append(bag_a_space);
    }
}


