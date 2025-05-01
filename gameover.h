#ifndef GAMEOVER_H
#define GAMEOVER_H


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
//#include <QDebug>
#include <QTimer>
#include <QGraphicsOpacityEffect>


class GameOver : public QWidget
{
    Q_OBJECT

public:
    explicit GameOver(QWidget *parent = nullptr);

signals:



private:
    QLabel *background; // 背景圖片

};

#endif // TITLESCREEN_H
