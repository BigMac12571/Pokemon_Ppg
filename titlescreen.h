#ifndef TITLESCREEN_H
#define TITLESCREEN_H


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
//#include <QDebug>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QMediaPlayer>
#include <QUrl>
#include <QMediaContent>


class TitleScreen : public QWidget
{
    Q_OBJECT

public:
    explicit TitleScreen(QWidget *parent = nullptr);
    ~TitleScreen();

signals:
    void Game_Start();  // ✅ 宣告 signal


private:

    QLabel *background; // 背景圖片
    QPushButton *press_start;
    QTimer *timer;
    QMediaPlayer *backgroundMusicPlayer;
    bool LoopMusic = true;
};

#endif // TITLESCREEN_H
