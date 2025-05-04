#ifndef NPC_H
#define NPC_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
//#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QGraphicsOpacityEffect>

enum DirectionNPC {
    up = 0,
    down = 1,
    left1 = 2,
    right1 = 3,
};
enum WalkFrameNPC {
    STOP = 0,
    WALK1 = 1,
    WALK2 = 2,
};

class NPC: public QLabel
{
    Q_OBJECT

public:

    explicit NPC(QWidget *parent = nullptr);
    void setImage(const QString &path);

    void setDirection(DirectionNPC dir);
    void nextWalkFrame(); // 換走路動畫圖
    void startWalking(); //開始走路(用於動畫)
    void stopWalking(); //停止走路(用於動畫)
    bool isWalking();
    QPoint mapPosition; // NPC 在地圖上的真實座標 (浮點數可以更精細地控制移動)

    void setMapPosition(const QPoint &pos);
    QPoint getMapPosition() const;

public slots:
    void updateWalkFrame();  // 0.5 秒換腳




private:

    QLabel *NPC_photo; // 角色圖片
    QTimer *walkTimer; // 0.5 秒換腳

    DirectionNPC currentDirection = down;
    WalkFrameNPC walkFrame ; // 0: 靜止, 1: 走路圖1, 2: 走路圖2
    WalkFrameNPC lastWalk;
    bool Walking ; //是否在walking
    QVector<QVector<QString>> imagePaths;;

};

#endif // TITLESCREEN_H
