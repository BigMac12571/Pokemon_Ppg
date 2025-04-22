#ifndef DIALOG_H
#define DIALOG_H


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QStringList>
#include "player.h"




class Dialog:public QLabel
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);


    void Oak_Dialog();
    void Sign_Dialog();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void Close_Dialog();
private:

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度



    QStringList Oak_dialog;
    bool Oak_dialog_start;
    QStringList Sign_dialog;
    bool Sign_dialog_start;
    int CurrentDialog;





};

#endif // BAG_H
