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




class Dialog:public QLabel
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);


    void Oak_Dialog();
    void Sign_Dialog();
    void Grassland_Dialog();


    void Show_Pokeballs_Dialog(int id);
    //void Setup_Shared_pokeball_ID(int id);


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void Close_Dialog();

    void Pickup_Pokeballs(int id);
private:

    const int View_Width = 525; // 視窗寬度
    const int View_Height = 450; // 視窗高度



    QStringList Oak_dialog;
    bool Oak_dialog_start = false;
    QStringList Sign_dialog;

    bool Sign_dialog_start = false;
    QStringList Grassland_dialog;
    bool Grassland_dialog_start = false;

    QList<QStringList> Pickup_Pokeballs_dialog;
    bool Pickup_Pokeballs_dialog_start = false;
    int Shared_pokeball_ID;
    bool Waiting_For_YesNo;


    int CurrentDialog;





};

#endif // BAG_H
