#include "town.h"


Town::Town(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(Map_Width,Map_Height);

    Map_Offset = QPoint(Player_Center_X, Player_Center_X); //Map_Offset位置

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/Town.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-Player_Center_X, -Player_Center_X, width(), height()); // 填滿整個視窗
    background->lower(); //背景在最下


    OpenBag = false; //背包關著

    // 加入地圖邊界的樹木（以整張背景為 1000x1000 計算）
    Barriers.append(QRect(0, 0, 480, 80));    // 上邊界
    Barriers.append(QRect(600, 0, 400, 80));    // 上邊界2
    Barriers.append(QRect(0,980, 1000,20));  // 下邊界
    Barriers.append(QRect(0, 0, 80, 1000));    // 左邊界
    Barriers.append(QRect(920, 0, 80, 1000));  // 右邊界

    // 加入中間的房子、柵欄等（你可以根據 Town.png 的實際位置微調）
    Barriers.append(QRect(207, 173, 209, 210)); // 左上房子
    Barriers.append(QRect(588, 173, 209, 210));  // 右上房子
    Barriers.append(QRect(542, 474, 284 , 210)); //右下房子
    Barriers.append(QRect(207, 558, 209 , 32)); //花旁柵欄
    Barriers.append(QRect(544, 808, 209 , 32)); //右下柵欄
    Barriers.append(QRect(294, 851, 164 , 149)); //水池
    Barriers.append(QRect(172, 340, 47 , 43)); //油箱左
    Barriers.append(QRect(550, 340, 47 , 43)); //油箱右

    Enter_Laboratory_Trigger = QRect(668, 645, 36, 50);  //實驗室大門
    Enter_Grassland_Trigger = QRect(480,0, 120, 1);







    setFocusPolicy(Qt::StrongFocus);
}
void Town::Add_Player_To_Scene(QWidget *player) //可以同時出現Town 與 Player
{
    player->setParent(this); //設定 player 的父元件 //player 會被加到 this（也就是 Town）的 widget 裡，這樣它才會顯示在畫面上。
    player->setGeometry(Player_Center_X, Player_Center_Y, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方

}

void Town::SetMainPlayer(Player *p) {
    mainPlayer = p; //p 指向 mainPlayer 這個物件

    mainPlayer->move(Player_Center_X, Player_Center_Y);
    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();

    keysPressed.clear(); // 清空按鍵狀態

}
void Town::SetMainPlayer_GrasslandToTown(Player *p) {
    mainPlayer = p; //p 指向 mainPlayer 這個物件

    mainPlayer->move(Player_Center_X, Player_Center_Y-200); //要有Player_Center_Y否則因為每一步是5像素所以永遠到不了中央
    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();

    keysPressed.clear(); // 清空按鍵狀態

}



void Town::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();

    // 防止 auto-repeat
    if (keysPressed.contains(key)) return;
    keysPressed.insert(key);


    // 如果背包打開，不允許移動
    if (OpenBag && (key == Qt::Key_Up || key == Qt::Key_Down || key == Qt::Key_Left || key == Qt::Key_Right))return;




    int Step = 5;

    switch (key) {
    case Qt::Key_Up:{
        if (CanMoveToDirection(UP)) {
            if (Map_Offset.y() > 0) { // 如果背景還能往上捲
                if (mainPlayer->y() == Player_Center_Y) {
                    // 角色在中心，讓背景捲動
                    Map_Offset.setY(Map_Offset.y() - Step);
                    UpdateScene();
                } else {
                    // 角色不在中心，角色移動(用於到邊邊要往回走(例如這個就是為了到下邊邊後可以往上走，也符合上捲)
                    mainPlayer->move(mainPlayer->x(), mainPlayer->y() - Step);
                }
            } else {
                // 背景已到頂，角色自己移動（不能超出畫面）
                if (mainPlayer->y() > 0) {
                    mainPlayer->move(mainPlayer->x(), mainPlayer->y() - Step);
                }
            }
        }
        QRect playerRect = mainPlayer->geometry();
        QRect Real_coodinate = playerRect.translated(Map_Offset); // 角色在地圖中的實際位置

        if (Enter_Laboratory_Trigger.intersects(Real_coodinate)) {
            emit Enter_Laboratory();
        }
        if (Enter_Grassland_Trigger.intersects(Real_coodinate)) {
            emit Enter_Grassland();
        }

        mainPlayer->setDirection(UP);
        mainPlayer->startWalking();
        break;
    }

    case Qt::Key_Down:
        if (CanMoveToDirection(DOWN)) {
            if (Map_Offset.y() < Map_Height - View_Height) { // 背景還能往下捲
                if (mainPlayer->y() == Player_Center_Y) {
                    // 角色在中心，捲動背景
                    Map_Offset.setY(Map_Offset.y() + Step);
                    UpdateScene();
                } else {
                    // 角色不在中心，角色自己走
                    mainPlayer->move(mainPlayer->x(), mainPlayer->y() + Step);
                }
            } else {
                // 地圖已到最底，角色自己動
                if (mainPlayer->y() < View_Height - mainPlayer->height()) {
                    mainPlayer->move(mainPlayer->x(), mainPlayer->y() + Step);
                }
            }
        }
        mainPlayer->setDirection(DOWN);
        mainPlayer->startWalking();
        break;

    case Qt::Key_Left:
        if (CanMoveToDirection(LEFT)) {
            if (Map_Offset.x() > 0) { // 背景還能往左捲
                if (mainPlayer->x() == Player_Center_X) {
                    // 角色在中心，捲動背景
                    Map_Offset.setX(Map_Offset.x() - Step);
                    UpdateScene();
                } else {
                    // 角色不在中心，角色自己走
                    mainPlayer->move(mainPlayer->x() - Step, mainPlayer->y());
                }
            } else {
                // 地圖已到最左邊，角色自己動
                if (mainPlayer->x() > 0) {
                    mainPlayer->move(mainPlayer->x() - Step, mainPlayer->y());
                }
            }
        }
        mainPlayer->setDirection(LEFT);
        mainPlayer->startWalking();
        break;

    case Qt::Key_Right:
        if (CanMoveToDirection(RIGHT)) {
            if (Map_Offset.x() < Map_Width - View_Width) { // 背景還能往右捲
                if (mainPlayer->x() == Player_Center_X) {
                    // 角色在中心，捲動背景
                    Map_Offset.setX(Map_Offset.x() + Step);
                    UpdateScene();
                } else {
                    // 角色不在中心，角色自己動
                    mainPlayer->move(mainPlayer->x() + Step, mainPlayer->y());
                }
            } else {
                // 地圖已到最右邊，角色自己動
                if (mainPlayer->x() < View_Width - mainPlayer->width()) {
                    mainPlayer->move(mainPlayer->x() + Step, mainPlayer->y());
                }
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

    }



}


void Town::keyReleaseEvent(QKeyEvent *event)
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

void Town::UpdateScene()
{
    background->move(-Map_Offset.x(), -Map_Offset.y()); // 移動背景
}
bool Town::CanMoveToDirection(Direction dir)
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


