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


    // 加入地圖邊界的樹木（以整張背景為 1000x1000 計算）
    Barriers.append(QRect(678, 437, Map_Width, 1));    // 上邊界
    Barriers.append(QRect(680, 875, Map_Width,1));  // 下邊界
    Barriers.append(QRect(678, 437, 1, Map_Height));    // 左邊界
    Barriers.append(QRect(1134, 437, 1, Map_Height));  // 右邊界

    // 加入中間的房子、柵欄等（你可以根據 Town.png 的實際位置微調）
    //Barriers.append(QRect(207, 173, 209, 210)); // 左上房子
    //Barriers.append(QRect(588, 173, 209, 210));  // 右上房子
    //Barriers.append(QRect(542, 474, 284 , 223)); //右下房子
    //Barriers.append(QRect(207, 558, 209 , 32)); //花旁柵欄
    //Barriers.append(QRect(544, 808, 209 , 32)); //右下柵欄
    //Barriers.append(QRect(294, 851, 164 , 149)); //水池
    //Barriers.append(QRect(172, 340, 47 , 43)); //油箱左
    //Barriers.append(QRect(550, 340, 47 , 43)); //油箱右

    Exit_Zone = QRect(879, 863, 50, 30); // 自己依照背景圖微調







    setFocusPolicy(Qt::StrongFocus);
}
void Laboratory::Add_Player_To_Scene(QWidget *player) //可以同時出現Town 與 Player
{
    player->setParent(this); //設定 player 的父元件 //player 會被加到 this（也就是 Town）的 widget 裡，這樣它才會顯示在畫面上。
    player->setGeometry(Player_Center_X, Player_Center_Y, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方

}

void Laboratory::SetMainPlayer(Player *p) {
    mainPlayer = p; //p 指向 mainPlayer 這個物件

    mainPlayer->move(Player_Center_X, Player_Center_Y);
    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();
}

void Laboratory::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();

    // 防止 auto-repeat
    if (keysPressed.contains(key)) return;
    keysPressed.insert(key);

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

    }
}


void Laboratory::keyReleaseEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr) return;

    int key = event->key();
    keysPressed.remove(key);

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
