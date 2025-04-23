#include "laboratory.h"

Laboratory::Laboratory(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(Black_Width,Black_Height);



    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/lab.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-(View_Coordinate_x-(View_Width-Map_Width)/2), -(View_Coordinate_y-Player_Center_Y+Map_Height-100), width(), height()); // 填滿整個視窗
    background->lower(); //背景在最下

    Map_Offset = QPoint(View_Coordinate_x-(View_Width-Map_Width)/2, View_Coordinate_y-Player_Center_Y+Map_Height-100); //Map_Offset位置
    //
    OpenBag = false;

    // 加入地圖邊界的樹木（以整張背景為 1000x1000 計算）
    Barriers.append(QRect(678, 437, Map_Width, 15));    // 上邊界
    Barriers.append(QRect(680, 875, Map_Width,1));  // 下邊界
    Barriers.append(QRect(678, 437, 1, Map_Height));    // 左邊界
    Barriers.append(QRect(1134, 437, 1, Map_Height));  // 右邊界

    // 加入中間的房子、柵欄等（你可以根據 Town.png 的實際位置微調）
    Barriers.append(QRect(681, 454, 888-681, 490-454)); // 左上一坨
    Barriers.append(QRect(998, 450, 1134-998, 490-454));  // 右上一坨
    Barriers.append(QRect(677, 525, 710-677 , 592-525)); //左邊機器一坨
    Barriers.append(QRect(709, 551, 784-709 , 626-551)); //左邊大機器一坨
    Barriers.append(QRect(960, 572, 1065-960 , 603-572)); //球桌一坨
    Barriers.append(QRect(678, 687, 853-678 , 733-687)); //左下一坨
    Barriers.append(QRect(960, 687, 1132-960 , 733-687)); //右下一坨
    Barriers.append(QRect(678, 817, 713-678 , 876-817)); //左下花盆一坨
    Barriers.append(QRect(1101, 817, 713-678 , 876-817)); //左下花盆一坨
    Barriers.append(QRect(893, 508, 27, 44)); //Oak哥


    Exit_Zone = QRect(904, 863, 4, 30); // 自己依照背景圖微調

    Talk_With_Oak =QRect(889,508,27, 64);

    Pick_Pokeballs_area.append(QRect(966,575,35,60));
    Pick_Pokeballs_area.append(QRect(997,575,35,60));
    Pick_Pokeballs_area.append(QRect(1035,575,35,60));





    setFocusPolicy(Qt::StrongFocus);
}
void Laboratory::Add_Player_To_Scene(QWidget *player) //可以同時出現Lab 與 Player
{
    player->setParent(this); //設定 player 的父元件 //player 會被加到 this（也就是 Lab）的 widget 裡，這樣它才會顯示在畫面上。
    player->setGeometry(Player_Center_X, Player_Center_Y, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方

}
void Laboratory::Add_NPC_To_Scene(NPC *npc) //可以同時出現Lab 與 NPC
{
    npc->setParent(this); //設定 player 的父元件 //player 會被加到 this（也就是 Lab）的 widget 裡，這樣它才會顯示在畫面上。
    npc->setGeometry(-Map_Offset.x()+889, -Map_Offset.x()+508, 35, 48);
    npc->show();
    npc->raise(); // 確保角色在背景上方
    mainPlayer->raise();
    ProfessorOak = npc;

}
void Laboratory::Add_Pokeball_To_Scene(int id,Pokeball *pokeballx){
    if (!pokeballx) return;

    pokeballx->setParent(this); // 设置父对象为 Laboratory

    int x = 0;
    int y = 0;

    // 根据 Pokeball 的 ID 设置它在实验室内地图上的绝对位置 (请根据 lab.png 实际情况修改这些坐标)
    if (id==0) {
        x = 966; y = 575;
    } else if (id==1) {
        x = 1000; y = 575;
    } else if (id==2) {
        x = 1035; y = 575;
    } else {
        qDebug() << "Warning: Add_Pokeball_To_Scene received pokeball with unexpected ID:" << pokeballx->ID;
        pokeballx->hide();
        return;
    }

    // 设置 Pokeball Widget 在 Laboratory 视窗中的位置
    pokeballx->setGeometry(- Map_Offset.x()+x, - Map_Offset.y()+y, pokeballx->width(), pokeballx->height());

    pokeballx->raise();
    pokeballx->show();


    if (!pokeball.contains(pokeballx)) {
         pokeball.append(pokeballx);
    }
}

void Laboratory::Pokeball_get_picked(Pokeball *pokeballx){

    pokeballx->hide();
}




void Laboratory::SetMainPlayer(Player *p) {
    mainPlayer = p; //p 指向 mainPlayer 這個物件

    mainPlayer->move(Player_Center_X, Player_Center_Y);
    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();
    keysPressed.clear(); // 清空按鍵狀態
}

void Laboratory::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();

    // 防止 auto-repeat
    if (keysPressed.contains(key)) return;
    keysPressed.insert(key);

    // 如果背包打開，不允許移動
    if ((OpenBag) && (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Left || key == Qt::Key_Right))return;

    int Step = 5;

    switch (key) {
    case Qt::Key_Up:
        if (CanMoveToDirection(UP)) {
            if (mainPlayer->y() == Player_Center_Y) {
                    // 角色在中心，讓背景捲動
                    Map_Offset.setY(Map_Offset.y() - Step);
                    UpdateScene();
                }

        }
        mainPlayer->setDirection(UP);
        mainPlayer->startWalking();
        break;

    case Qt::Key_Down:{
        if (CanMoveToDirection(DOWN)) {

                if (mainPlayer->y() == Player_Center_Y) {
                    // 角色在中心，捲動背景
                    Map_Offset.setY(Map_Offset.y() + Step);
                    UpdateScene();
                }
            }
        QRect playerRect = mainPlayer->geometry();
        QRect Real_coodinate = playerRect.translated(Map_Offset); // 角色在地圖中的實際位置

        if (Exit_Zone.intersects(Real_coodinate)) {
            emit Exit_Laboratory();
        }
        mainPlayer->setDirection(DOWN);
        mainPlayer->startWalking();

        break;}

    case Qt::Key_Left:
        if (CanMoveToDirection(LEFT)) {

                if (mainPlayer->x() == Player_Center_X) {
                    // 角色在中心，捲動背景
                    Map_Offset.setX(Map_Offset.x() - Step);
                    UpdateScene();
                }

            }

        mainPlayer->setDirection(LEFT);
        mainPlayer->startWalking();
        break;

    case Qt::Key_Right:
        if (CanMoveToDirection(RIGHT)) {

                if (mainPlayer->x() == Player_Center_X) {
                    // 角色在中心，捲動背景
                    Map_Offset.setX(Map_Offset.x() + Step);
                    UpdateScene();
                }
            }
        mainPlayer->setDirection(RIGHT);
        mainPlayer->startWalking();
        break;

    case Qt::Key_B:
        if (OpenBag) {
            bag->hide();
            OpenBag = false;
        } else {
            if (bag == nullptr) {
                bag = new Bag(this); // 設定 parent，這樣它會跟隨 Town 視窗
            }
            bag->show();
            bag->raise(); // 確保在最上層
            OpenBag = true;
            mainPlayer->stopWalking();
        }

        break;
    case Qt::Key_A: {

        QRect playerRect = mainPlayer->geometry();
        QRect Real_coodinate = playerRect.translated(Map_Offset); // 真實地圖上的位置
            if (Talk_With_Oak.intersects(Real_coodinate)) {
                emit Open_Dialog_Oak();  // 觸發對話 signal
                mainPlayer->stopWalking();
                //qDebug() << "Player rect: " << Real_coodinate << " Talk zone: " << Talk_With_Oak;
        }
            for(int i=0;i<Pick_Pokeballs_area.size();i++){
                if(Pick_Pokeballs_area[i].intersects(Real_coodinate)){
                    emit Pickup_Pokeballs(i);

                }
            }

        break;
    }

    }
}


void Laboratory::keyReleaseEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();
    keysPressed.remove(key);
    // 背包打開時不處理釋放
    if (OpenBag && (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Left || key == Qt::Key_Right))return;

    switch (key) {
    case Qt::Key_Up:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(UP);
        }
        break;
    case Qt::Key_Down:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(DOWN);
        }
        break;
    case Qt::Key_Left:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(LEFT);
        }
        break;
    case Qt::Key_Right:
        if (keysPressed.isEmpty()) {
            mainPlayer->stopWalking();
        } else {
            mainPlayer->setDirection(RIGHT);
        }
        break;
    default:
        break;
    }
}

void Laboratory::UpdateScene()
{
    background->move(-Map_Offset.x(), -Map_Offset.y()); // 移動背景
    if (ProfessorOak) {
            ProfessorOak->move(-Map_Offset.x()+889, -Map_Offset.y()+508);
        }
    for (int i = 0; i < pokeball.size(); i++) {
        if (!pokeball[i]) continue; // 保險一點，避免 nullptr crash
        switch(i){
        case 0: pokeball[i]->move(-Map_Offset.x() + 966, -Map_Offset.y() + 575); break;
        case 1: pokeball[i]->move(-Map_Offset.x() + 1000, -Map_Offset.y() + 575); break;
        case 2: pokeball[i]->move(-Map_Offset.x() + 1035, -Map_Offset.y() + 575); break;
        }
    }
}

bool Laboratory::CanMoveToDirection(Direction dir)
{
    QRect playerRect = mainPlayer->geometry();
    QRect movedRect = playerRect;

    const int Step = 10; //模擬下一步(只有下一步，不是每一步，等於是在正前Step的距離方有個替身

    if (dir == UP) movedRect.translate(0, -Step);
    else if (dir == DOWN) movedRect.translate(0, Step);
    else if (dir == LEFT) movedRect.translate(-Step, 0);
    else if (dir == RIGHT) movedRect.translate(Step, 0);

    // 加上 Map_Offset 把它轉換成背景座標
    movedRect.translate(Map_Offset);

    // 縮小碰撞框讓角色更寬容
    movedRect.adjust(2, 2, -2, -2);

    for (const QRect &barrier : Barriers) {
        if (movedRect.intersects(barrier)) {
            return false;
        }
    }

    return true;
}
