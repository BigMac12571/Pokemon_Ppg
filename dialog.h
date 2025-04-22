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


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度



    QStringList Oak_dialog;
    int CurrentDialog;






};

#endif // BAG_H
