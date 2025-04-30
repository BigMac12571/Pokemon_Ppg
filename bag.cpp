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
        for (int col = 0; col < 3; ++col) {
            QWidget* placeholder = new QWidget(this);
            placeholder->setStyleSheet("background: transparent;");
            Menu->addWidget(placeholder, row, col);
            rowPlaceholders.append(placeholder);
        }
        pokemonPlaceholders.append(rowPlaceholders);
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
    delete ImageLabel;
    delete NameLabel;
    delete LevelLabel;
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
    PokemonData new_pokemon(id, level);

    int emptyIndex = -1;
    for (int i = 0; i < Pokemon_List.size(); ++i) {
        if (Pokemon_List[i].isEmpty()) {
            emptyIndex = i;
            break;
        }
    }

    if (emptyIndex != -1) {
        Pokemon_List[emptyIndex] = new_pokemon;
    } else {
        Pokemon_List.append(new_pokemon);
    }

    Refresh_bag_pokemon();
    //    QPixmap image = new_pokemon.GetImagePath();
    //    Pokemon_image.at(id)->setPixmap(image.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //    Pokemon_name.at(id)->setStyleSheet("color: black; font-size: 30px;");
    //    Pokemon_name.at(id)->setText(new_pokemon.GetName());

}

void Bag::Remove_Pokemon(int id) {
    if (id >= 0 && id < Pokemon_List.size()) {
        Pokemon_List[id] = PokemonData(); // 替換為空的 PokemonData
        Refresh_bag_pokemon(); // 更新背包的寶可夢顯示
    }
}

void Bag::UsePokeball(){
    if(pokeball>=0){

        pokeball --;
    }

    Refresh_bag(0);

}
void Bag::UsePotion(){
    if(potion>=0){

        potion --;
    }

    Refresh_bag(1);

}
void Bag::UseEther(){
    if(ether>=0){

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

void Bag::Refresh_bag_pokemon() {
    for (int row = 0; row < pokemonPlaceholders.size(); ++row) {
        for (int col = 0; col < pokemonPlaceholders[row].size(); ++col) {
            QLabel* currentImageLabel = nullptr;
            QLabel* currentNameLabel = nullptr;
            QLabel* currentLevelLabel = nullptr;

            currentImageLabel = ImageLabel;
            currentNameLabel = NameLabel;
            currentLevelLabel = LevelLabel;


            if(currentImageLabel){
                //            Menu->addWidget(nameLabel, row, 1);
                if (row < Pokemon_List.size()) {
                    const PokemonData& pokemon = Pokemon_List[row];
                    if (!pokemon.isEmpty()) {
                        // 獲取當前等級對應的圖像和名字
                        QPixmap image = pokemon.GetImagePath();

                        // 更新圖片 QLabel
                        currentImageLabel->setPixmap(image.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        currentImageLabel->setAlignment(Qt::AlignCenter);
                        Menu->addWidget(currentImageLabel, row, 0);

                    }else {
                        // 顯示透明佔位符
                        Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                    }
                }else {
                    // 超出 Pokemon_List 大小的位置顯示透明佔位符
                    Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                }
            }else{
                if (row < Pokemon_List.size()) {
                    const PokemonData& pokemon = Pokemon_List[row];
                    if (!pokemon.isEmpty()) {
                        // 獲取當前等級對應的圖像和名字
                        QPixmap image = pokemon.GetImagePath();

                        // 更新圖片 QLabel
                        QLabel* imageLabel = new QLabel(this);
                        imageLabel->setPixmap(image.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        imageLabel->setAlignment(Qt::AlignCenter);
                        Menu->addWidget(imageLabel, row, 0);

                        ImageLabel = imageLabel;

                    }else {
                        // 顯示透明佔位符
                        Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                    }
                }else {
                    // 超出 Pokemon_List 大小的位置顯示透明佔位符
                    Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                }
            }
            if(currentNameLabel){
                //            Menu->addWidget(nameLabel, row, 1);
                if (row < Pokemon_List.size()) {
                    const PokemonData& pokemon = Pokemon_List[row];
                    if (!pokemon.isEmpty()) {
                        // 獲取當前等級對應的圖像和名字
                        QString name = pokemon.GetName();

                        // 更新圖片 QLabel
                        currentNameLabel->setStyleSheet("color: black; font-size: 30px;");
                        currentNameLabel->setText(name);
                        currentNameLabel->setAlignment(Qt::AlignVCenter);
                        Menu->addWidget(currentNameLabel, row, 1);

                    }else {
                        // 顯示透明佔位符
                        Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                    }
                }else {
                    // 超出 Pokemon_List 大小的位置顯示透明佔位符
                    Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                }
            }else{
                if (row < Pokemon_List.size()) {
                    const PokemonData& pokemon = Pokemon_List[row];
                    if (!pokemon.isEmpty()) {
                        // 獲取當前等級對應的圖像和名字
                        QString name = pokemon.GetName();

                        // 更新圖片 QLabel
                        QLabel* nameLabel = new QLabel(this);
                        nameLabel->setStyleSheet("color: black; font-size: 30px;");
                        nameLabel->setText(name);
                        nameLabel->setAlignment(Qt::AlignVCenter);
                        Menu->addWidget(nameLabel, row, 1);

                        NameLabel = nameLabel;

                    }else {
                        // 顯示透明佔位符
                        Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                    }
                }else {
                    // 超出 Pokemon_List 大小的位置顯示透明佔位符
                    Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                }
            }
            if(currentLevelLabel){
                //            Menu->addWidget(nameLabel, row, 1);
                if (row < Pokemon_List.size()) {
                    const PokemonData& pokemon = Pokemon_List[row];
                    if (!pokemon.isEmpty()) {
                        // 獲取當前等級對應的圖像和名字
                        int level = pokemon.GetLevel();

                        // 更新圖片 QLabel
                        currentLevelLabel->setStyleSheet("color: black; font-size: 25px;");
                        currentLevelLabel->setText("LV:" + QString::number(level));
                        currentLevelLabel->setAlignment(Qt::AlignVCenter);
                        Menu->addWidget(currentLevelLabel, row, 2);

                    }else {
                        // 顯示透明佔位符
                        Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                    }
                }else {
                    // 超出 Pokemon_List 大小的位置顯示透明佔位符
                    Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                }
            }else{
                if (row < Pokemon_List.size()) {
                    const PokemonData& pokemon = Pokemon_List[row];
                    if (!pokemon.isEmpty()) {
                        // 獲取當前等級對應的圖像和名字
                        int level = pokemon.GetLevel();

                        // 更新圖片 QLabel
                        QLabel* levelLabel = new QLabel(this);
                        levelLabel->setStyleSheet("color: black; font-size: 25px;");
                        levelLabel->setText("LV:" + QString::number(level));
                        levelLabel->setAlignment(Qt::AlignVCenter);
                        Menu->addWidget(levelLabel, row, 2);

                        LevelLabel = levelLabel;

                    }else {
                        // 顯示透明佔位符
                        Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                    }
                }else {
                    // 超出 Pokemon_List 大小的位置顯示透明佔位符
                    Menu->addWidget(pokemonPlaceholders[row][col], row, col);
                }
            }
        }
    }
}

