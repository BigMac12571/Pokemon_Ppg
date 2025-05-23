#include "grassland.h"

Grassland::Grassland(Bag *mybag,QWidget *parent)
    : QWidget(parent),bag(mybag)
{
    this->setFixedSize(Map_Width,Map_Height);



    background = new QLabel(this);
    QPixmap backgroundPixmap(":/new/prefix1/Dataset/Image/scene/GrassLand.png");
    background->setPixmap(backgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    background->setGeometry(-(View_Width/2+15),-(Map_Height-View_Height), width(), height()); // 填滿整個視窗
    background->lower(); //背景在最下
    Map_Offset = QPoint(View_Width/2+15, Map_Height-View_Height); //Map_Offset位置

    OpenBag = false;

    // 加入地圖邊界的樹木（以整張背景為 1000x1667 計算）
    Barriers.append(QRect(0, 0, 414, 80));    // 上邊界
    Barriers.append(QRect(586, 0, 414, 80));    // 上邊界2
    Barriers.append(QRect(334,132, 80, 364));  // 上直樹4
    Barriers.append(QRect(0, 0, 80, 1667));    // 左邊界
    Barriers.append(QRect(920, 0, 80, 1667));  // 右邊界
    Barriers.append(QRect(86,590, 80,116));  // 中橫樹1
    Barriers.append(QRect(418,590, 246,116));  // 中橫樹3
    Barriers.append(QRect(86,1006, 410,116));  // 下橫樹5
    Barriers.append(QRect(0,1504, 488,163));  // 下邊界+柵欄
    Barriers.append(QRect(594,1504, 418,163));  // 下邊界+柵欄2
    Barriers.append(QRect(374,1294, 42,20));  // 公佈欄

    //加入壁崖
    Ledges.append(QRect(82,228,250,20));//壁崖(1,1)
    Ledges.append(QRect(416,228,250,20));//壁崖(1,2)
    Ledges.append(QRect(82,436,250,20));//壁崖(2,1)
    Ledges.append(QRect(166,644,250,20));//壁崖(3,1)
    Ledges.append(QRect(82,852,82,20));//壁崖(4,1)
    Ledges.append(QRect(208,852,166,20));//壁崖(4,2)
    Ledges.append(QRect(456,852,458,20));//壁崖(4,3)
    Ledges.append(QRect(750,1102,166,20));//壁崖(5,1)
    Ledges.append(QRect(82,1310,166,20));//壁崖(6,1)
    Ledges.append(QRect(416,1310,500,20));//壁崖(6,2)

    //加入高草區
    TallGrasses.append(QRect(436, 270,460,166));//上長高草
    TallGrasses.append(QRect(686, 562,208,166));//上短高草
    TallGrasses.append(QRect(520, 1020,208,166));//中短高草
    TallGrasses.append(QRect(104, 1354,332,124));//左階梯形高草
    TallGrasses.append(QRect(644, 1354,252,124));//右階梯型高草
    TallGrasses.append(QRect(520, 1478,42,166));//下長高草

    Exit_Zone = QRect(500, 1642, 84, 24); // 自己依照背景圖微調
    Talk_With_Sign.append(QRect(374,1294,42,50)); //高草旁Sign

    battleFlashTimer = new QTimer(this);
    connect(battleFlashTimer, &QTimer::timeout, this, &Grassland::handleBattleFlashTimeout);
    battleFlashTimer->setSingleShot(false); // 不是單次觸發

    tung_tung_tung_sahur = new QLabel(this);
    QPixmap tung_image(":/new/prefix1/Dataset/Image/battle/tung_tung_tung_sahur-removebg-preview.png");
    tung_tung_tung_sahur->setPixmap(tung_image.scaled(40, 50, Qt::KeepAspectRatio));
    tung_tung_tung_sahur->setGeometry(472-Map_Offset.x(),31-Map_Offset.y(), 40, 50);
    tung_tung_tung_sahur->show();
    tung_tung_tung_sahur->raise();
    fight_with_tung = false;
    tung_times = 0;

    tung = QRect(472, 31,40,50);


    setFocusPolicy(Qt::StrongFocus);
}

Grassland::~Grassland(){
//    if (backgroundMusicPlayer) {
//        backgroundMusicPlayer->stop();
//        delete backgroundMusicPlayer;
//        backgroundMusicPlayer = nullptr;
//    }
}

void Grassland::Add_Player_To_Scene(QWidget *player) //可以同時出現Town 與 Player
{
    player->setParent(this); //設定 player 的父元件 //player 會被加到 this（也就是 Town）的 widget 裡，這樣它才會顯示在畫面上。
    player->setGeometry(Player_Center_X, Player_Center_Y, 35, 48);
    player->show();
    player->raise(); // 確保角色在背景上方

    backgroundMusicPlayer = new QMediaPlayer(this);
    QUrl musicUrl = QUrl("qrc:/new/prefix2/Dataset/sound/GRASSLAND.wav");
    backgroundMusicPlayer->setMedia(QMediaContent(musicUrl));
    backgroundMusicPlayer->setVolume(15);
    backgroundMusicPlayer->play();
    LoopMusic = true;

    connect(backgroundMusicPlayer, &QMediaPlayer::stateChanged, this,
            [this](QMediaPlayer::State newState){
        if (newState == QMediaPlayer::StoppedState && LoopMusic) {
            backgroundMusicPlayer->play(); // 播放結束後重新開始
        }
    });

}

void Grassland::SetMainPlayer(Player *p) {
    mainPlayer = p; //p 指向 mainPlayer 這個物件

    mainPlayer->move(Player_Center_X, Player_Center_Y+175);



    mainPlayer->raise();
    mainPlayer->setFocus();
    this->setFocus();
    keysPressed.clear(); // 清空按鍵狀態

}
void Grassland::clearPressedKeys() {
    keysPressed.clear();
}

void Grassland::keyPressEvent(QKeyEvent *event)
{
    if (mainPlayer == nullptr|| isBattleFlashing) return;

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

        mainPlayer->setDirection(UP);
        mainPlayer->startWalking();
        EncounterBattle();
        break;}

    case Qt::Key_Down:{
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
        QRect playerRect = mainPlayer->geometry();
        QRect Real_coodinate = playerRect.translated(Map_Offset); // 角色在地圖中的實際位置

        if (Exit_Zone.intersects(Real_coodinate)) {
            if (backgroundMusicPlayer && backgroundMusicPlayer->state() == QMediaPlayer::PlayingState) {
                LoopMusic = false;
                backgroundMusicPlayer->stop();
            }
            emit Exit_Grassland();
        }
        mainPlayer->setDirection(DOWN);
        mainPlayer->startWalking();
        EncounterBattle();
        break;
    }

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
        EncounterBattle();
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
        EncounterBattle();
        break;
    case Qt::Key_B:
        if(OpenBag) OpenBag = false;
        else {OpenBag = true;}
        emit Open_Bag_Signal();
        emit Refresh_bag();
        mainPlayer->stopWalking();

        break;


    case Qt::Key_A:
    {

            QRect playerRect = mainPlayer->geometry();
            QRect Real_coodinate = playerRect.translated(Map_Offset); // 真實地圖上的位置
            for(int i=0;i< Talk_With_Sign.size();i++){
                if (Talk_With_Sign[i].intersects(Real_coodinate)) {
                    emit Open_Dialog_Grassland_Sign();  // 觸發對話 signal
                    mainPlayer->stopWalking();

                }
            }
            break;
        }


    }
}


void Grassland::keyReleaseEvent(QKeyEvent *event){
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

void Grassland::UpdateScene()
{
    background->move(-Map_Offset.x(), -Map_Offset.y()); // 移動背景

    if(tung_tung_tung_sahur) tung_tung_tung_sahur->move(472-Map_Offset.x(),31-Map_Offset.y());
}
bool Grassland::CanMoveToDirection(Direction dir)
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
    for(const QRect &ledge : Ledges){
        if(dir != DOWN && movedRect.intersects(ledge)){
            return false;
        }
    }

    return true;

}

void Grassland::EncounterBattle(){
    if(!bag->Pokemon_List.isEmpty()){

        QRect playerRect = mainPlayer->geometry();
        QRect Real_coodinate = playerRect.translated(Map_Offset); // 真實地圖上的位置

        if(mainPlayer->isWalking()){

            bool isInGrass = false;
            for (const QRect &tallgrass : TallGrasses) {
                if (tallgrass.intersects(Real_coodinate)) {
                    isInGrass = true;
                    break; // 只要與任何一個草叢相交，就認為在草叢中
                }
            }
            if(tung.intersects(Real_coodinate)){
                fight_with_tung = true;
            }
            if ((fight_with_tung ||isInGrass) && !Encountered) {
                Grass = true;
                //2%觸發戰鬥
                int random = QRandomGenerator::global()->generate() % 50;
                if(fight_with_tung) random =0 ;
                if (random == 0) {
                    //qDebug() << "fight!!!";
                    //emit Battle();
                    if (backgroundMusicPlayer && backgroundMusicPlayer->state() == QMediaPlayer::PlayingState) {
                        LoopMusic = false;
                        backgroundMusicPlayer->stop();
                    }
                    startBattleFlash(); // 觸發閃爍效果
                    Last_Map_Offset = Map_Offset;
                    Last_Player_Pos = mainPlayer->pos();
                    mainPlayer->stopWalking();
                    Encountered = true;
                    return; // 觸發戰鬥後直接返回，避免在同一步中多次觸發
                }
            } else if (!isInGrass && Grass) {
                fight_with_tung = false;
                Encountered = false;
                Grass = false;
            }

        }
    }
}
void Grassland::SetLastPosition() {
    if (Last_Map_Offset != QPoint()) {
        Map_Offset = Last_Map_Offset;
        background->move(-Map_Offset.x(), -Map_Offset.y());
    }

    if (mainPlayer && Last_Player_Pos != QPoint()) {
        mainPlayer->move(Last_Player_Pos);
    }
}

void Grassland::startBattleFlash()
{
    if (isBattleFlashing) return;

    isBattleFlashing = true;
    battleFlashCount = 0;
    battleFlashInterval = 150; // 閃爍間隔 200 毫秒

    originalBackgroundPixmap = *background->pixmap();

    // 創建閃爍效果的背景 (這裡簡單地讓背景稍微變暗)
    flashBackgroundPixmap = originalBackgroundPixmap;
    QPainter painter(&flashBackgroundPixmap);
    painter.fillRect(flashBackgroundPixmap.rect(), QColor(0, 0, 0, 90)); // 疊加半透明黑色
    painter.end();

    battleFlashTimer->start(battleFlashInterval);

    QSoundEffect *battleSoundEffect = new QSoundEffect(this);

    QUrl soundUrl = QUrl("qrc:/new/prefix2/Dataset/sound/Encountered.wav");
    battleSoundEffect->setSource(soundUrl);
    battleSoundEffect->setVolume(0.25f); // 設定音量 (0.0f - 1.0f)
    battleSoundEffect->play();

}

void Grassland::handleBattleFlashTimeout()
{
    if (battleFlashCount % 2 == 0) {
        background->setPixmap(flashBackgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        background->setPixmap(originalBackgroundPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    battleFlashCount++;

    if (battleFlashCount >= 8) { // 閃爍 5 次
        battleFlashTimer->stop();
        isBattleFlashing = false;
        if(fight_with_tung){

        emit Battle(true); // 發出開始戰鬥的訊號
            tung_times++;
            if(tung_times == 2){
        tung_tung_tung_sahur->hide();
        tung = QRect(0, 0,0,0);
            }
        } else emit Battle();
    }
}


