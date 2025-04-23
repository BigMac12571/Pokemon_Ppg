#include "town.h"

//#include <QDebug>

Town::Town(QWidget *parent)
    : QWidget(parent)
{

    this->setFixedSize(Map_Width,Map_Height);

    Map_Offset = QPoint(Player_Center_X-240, Player_Center_Y-2); //Map_Offset位置

    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/Town.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-Player_Center_X+240, -Player_Center_Y+2, width(), height()); // 填滿整個視窗
    background->lower(); //背景在最下


    OpenBag = false; //背包關著

    // 加入地圖邊界的樹木（以整張背景為 1000x1000 計算）
    Barriers.append(QRect(0, 0, 480, 80));    // 上邊界
    Barriers.append(QRect(600, 0, 400, 80));    // 上邊界2
    Barriers.append(QRect(0,980, 1000,20));  // 下邊界
    Barriers.append(QRect(0, 0, 80, 1000));    // 左邊界
    Barriers.append(QRect(920, 0, 80, 1000));  // 右邊界

    // 加入中間的房子、柵欄等（你可以根據 Town.png 的實際位置微調）
    Barriers.append(QRect(207, 173, 209, 221)); // 左上房子
    Barriers.append(QRect(584, 173, 208, 221));  // 右上房子
    Barriers.append(QRect(542, 474, 288 , 222)); //右下房子
    Barriers.append(QRect(210, 556, 204 , 20)); //花旁柵欄
    Barriers.append(QRect(544, 806, 244 , 20)); //右下柵欄
    Barriers.append(QRect(292, 850, 164 , 150)); //水池
    Barriers.append(QRect(172, 336, 30 , 58)); //油箱左
    Barriers.append(QRect(547, 336, 30 , 58)); //油箱右
    Barriers.append(QRect(210, 704, 36 , 20)); //木製公佈欄

    //加入不要生成box的地方
    NoBoxAreas.append(QRect(514,14,62,188));//Grassland入口
    NoBoxAreas.append(QRect(548,708,264,52));//lab門口
    NoBoxAreas.append(QRect(210,600,162,100));//花簇
    NoBoxAreas.append(QRect(376,600,40,98));//花旁公佈欄前
    NoBoxAreas.append(QRect(666,850,40,58));//右下公佈欄前
    NoBoxAreas.append(QRect(205,750,40,106));//木製公佈欄前


    Enter_Laboratory_Trigger = QRect(658, 696, 58, 8);  //實驗室大門
    Enter_Grassland_Trigger = QRect(480,0, 120, 1); //草地入口
    Talk_With_Sign.append(QRect(210,700,33,50)); //木Sign
    Talk_With_Sign.append(QRect(372,548,42,50)); //花旁柵欄Sign
    Talk_With_Sign.append(QRect(666,798,42,50)); //右下Sign

    //Enter_Laboratory_Trigger = QRect(684, 696, 4, 8);  //實驗室大門
    //Enter_Grassland_Trigger = QRect(480,0, 120, 1);
    //Talk_With_Sign.append(QRect(211,704,244-211,737-704));

    setFocusPolicy(Qt::StrongFocus);
    // 隨機生成 Box 物件
    int numberOfBoxes = 15; // 設定要生成的 Box 數量
    for (int i = 0; i < numberOfBoxes; ++i) {
        spawnBox();
    }
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
            if(OpenBag) OpenBag = false;
            else {OpenBag = true;}
            emit Open_Bag_Signal();
            mainPlayer->stopWalking();
        break;
    case Qt::Key_A:
    {
        QRect playerRect = mainPlayer->geometry();
        QRect Real_coodinate = playerRect.translated(Map_Offset); // 真實地圖上的位置

        // ✅ 處理與 Sign 對話
        for(int i = 0; i < Talk_With_Sign.size(); i++) {
            if (Talk_With_Sign[i].intersects(Real_coodinate)) {
                emit Open_Dialog_Sign();  // 觸發對話 signal
                mainPlayer->stopWalking();
            }
        }

        // ✅ 撿取箱子（如果與玩家碰撞）
        for (int i = 0; i < boxes.size(); ++i) {
            if (!boxes[i]->isPicked() && boxRects[i].intersects(Real_coodinate)) {
                boxes[i]->pick();  // 箱子撿起來並消失
                break; // 一次只撿一個箱子
            }
        }

        break;
    }





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

    // 重新定位 Box 物件，使其在視窗中看起來固定在世界座標上
        for (Box *box : boxes) {
            QRect boxWorldRect = boxRects[boxes.indexOf(box)]; // 獲取 Box 的世界座標矩形
            box->move(boxWorldRect.x() - Map_Offset.x(), boxWorldRect.y() - Map_Offset.y());
        }
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
void Town::spawnBox() {
    int x, y;
    QRect boxRect(0, 0, 33, 33);
    int maxAttempts = 100; // 防止無限迴圈

    for (int i = 0; i < maxAttempts; ++i) {
        // 在野外區域隨機生成座標 (根據你的 Town.png 調整)
        int minX = 0;
        int maxX = 1000 - boxRect.width();
        int minY = 0;
        int maxY = 1000 - boxRect.height();

        x = QRandomGenerator::global()->bounded(minX, maxX + 1);
        y = QRandomGenerator::global()->bounded(minY, maxY + 1);
        boxRect.moveTo(x, y);

        // 檢查新位置是否有效
        if (isPositionValid(boxRect)) {
            Box *box = new Box(this);
            box->move(x - Map_Offset.x(), y - Map_Offset.y()); // 相對於 Town 的位置
            box->show();
            box->raise();
            boxes.append(box);
            boxRects.append(boxRect);

            //qDebug() << "Box spawned at:" << boxRect;
            return;
        }
        //qDebug() << "Attempt " << i << " failed at:" << boxRect;
    }
    //qDebug() << "Failed to spawn box after " << maxAttempts << " attempts.";
}

bool Town::isPositionValid(const QRect& rect) {
    // 檢查是否與現有的障礙物重疊
    for (const QRect &barrier : Barriers) {
        if (rect.intersects(barrier)) {
            return false;
        }
    }
    // 檢查是否與已生成的 Box 重疊
    for (const QRect &existingBoxRect : boxRects) {
        if (rect.intersects(existingBoxRect)) {
            return false;
        }
    }
    //檢查是否生在 No Box Areas
    for (const QRect &noboxArea : NoBoxAreas) {
            if (rect.intersects(noboxArea)) {
                return false;
            }
        }
    // 檢查是否與玩家的起始位置重疊
    QRect playerStartRect(2*Player_Center_X-240, 2*Player_Center_Y-2, 35, 48);
    if (rect.intersects(playerStartRect)) {
        return false;
    }
    return true;
}


