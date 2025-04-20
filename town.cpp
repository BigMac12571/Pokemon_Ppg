#include "town.h"

Town::Town(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(1000,1000);

    Map_Offset = QPoint(Player_Center_X, Player_Center_X); //Map_Offset位置

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/Town.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-Player_Center_X, -Player_Center_X, width(), height()); // 填滿整個視窗

    // 加入地圖邊界的樹木（以整張背景為 1000x1000 計算）
    Barriers.append(QRect(0, 0, 1000, 50));    // 上邊界
    Barriers.append(QRect(0, 950, 1000, 50));  // 下邊界
    Barriers.append(QRect(0, 0, 50, 1000));    // 左邊界
    Barriers.append(QRect(950, 0, 50, 1000));  // 右邊界

    // 加入中間的房子、柵欄等（你可以根據 Town.png 的實際位置微調）
    //Barriers.append(QRect(255, 85, 200, 180)); // 左上房子
    //Barriers.append(QRect(545, 85, 200, 180));  // 右上房子
    //Barriers.append(QRect(530, 320, 315 , 185)); //右下房子
    //Barriers.append(QRect(310, 425, 170 , 20)); //花旁柵欄
    //Barriers.append(QRect(535, 585, 295 , 20)); //右下柵欄
    //Barriers.append(QRect(275, 670, 165 ,120)); //水池
    //Barriers.append(QRect(225, 200, 25 , 40)); //油箱左
    //Barriers.append(QRect(515, 200, 25 , 40)); //油箱右



    setFocusPolicy(Qt::StrongFocus);
}
void Town::Add_Player_To_Scene(QWidget *player) //可以同時出現Town 與 Player
{
    player->setParent(this); //設定 player 的父元件 //player 會被加到 this（也就是 Town）的 widget 裡，這樣它才會顯示在畫面上。
    player->setGeometry(525/2, 450/2, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方
}

void Town::SetMainPlayer(Player *p) {
    mainPlayer = p; //p 指向 mainPlayer 這個物件

    mainPlayer->move(Player_Center_X, Player_Center_Y);
    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();
}

void Town::keyPressEvent(QKeyEvent *event)
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
            if (Map_Offset.y() > 0) { // 如果背景還能往上捲
                if (mainPlayer->y() == Player_Center_Y) {
                    // 角色在中心，讓背景捲動
                    Map_Offset.setY(Map_Offset.y() - Step);
                    UpdateScene();
                } else {
                    // 角色不在中心，角色移動
                    mainPlayer->move(mainPlayer->x(), mainPlayer->y() - Step);
                }
            } else {
                // 背景已到頂，角色自己移動（不能超出畫面）
                if (mainPlayer->y() > 0) {
                    mainPlayer->move(mainPlayer->x(), mainPlayer->y() - Step);
                }
            }
        }
        mainPlayer->setDirection(UP);
        mainPlayer->startWalking();
        break;

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
    }
}


void Town::keyReleaseEvent(QKeyEvent *event)
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

void Town::UpdateScene()
{
    background->move(-Map_Offset.x(), -Map_Offset.y());// 把背景移動相反方向，讓主角看起來有在移動


}
bool Town::CanMoveToDirection(Direction dir)
{
    QRect playerRect = mainPlayer->geometry();
    int offsetX = Map_Offset.x();
    int offsetY = Map_Offset.y();
    int dx = 0, dy = 0;
    int Step = 3;

    if (dir == UP) dy = -Step;
    if (dir == DOWN) dy = Step;
    if (dir == LEFT) dx = -Step;
    if (dir == RIGHT) dx = Step;

    QRect movedRect(playerRect.x() + dx + offsetX + 2,
                    playerRect.y() + dy + offsetY + 2,
                    playerRect.width() - 4,
                    playerRect.height() - 4);

    for (const QRect &barrier : Barriers) {
        if (movedRect.intersects(barrier)) {
            return false;
        }
    }
    return true;
}
void Town::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    for (int i = 0; i < Barriers.size(); ++i) {
        const QRect &barrier = Barriers[i];
        QRect screenRect(barrier.x() - Map_Offset.x(), barrier.y() - Map_Offset.y(), barrier.width(), barrier.height());
        painter.drawRect(screenRect);
    }
}
