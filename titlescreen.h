#ifndef TITLESCREEN_H
#define TITLESCREEN_H


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QGraphicsOpacityEffect>


class TitleScreen : public QWidget
{
    Q_OBJECT

public:
    explicit TitleScreen(QWidget *parent = nullptr);

signals:
    void Game_Start();  // ✅ 宣告 signal


private:

    QLabel *background; // 背景圖片
    QPushButton *press_start;
    QTimer *timer;

};

#endif // TITLESCREEN_H
