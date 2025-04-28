#ifndef SQUIRTLE_H
#define SQUIRTLE_H


#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QGraphicsOpacityEffect>


class Squirtle: public QWidget {
    Q_OBJECT
public:
    enum Form { First, Second, Third };

    explicit Squirtle(QWidget* parent = nullptr);

    void SetForm(Form form);
    Form GetForm() ;
    QLabel *GetFormImage(Form form);
    QPixmap GetIconPixmap(const QSize& iconSize);


    void ShowFormImage(Form form, int x, int y);
    void HideFormImage(Form form);

private:
    Form Current_form;
    QLabel* FirstForm_Image;
    QLabel* SecondForm_Image;
    QLabel* ThirdForm_Image;

    int Level = 1;
    int ATK = 5;
    int DEF = 5;
    int maxHP = 30;
};


#endif // BULBASAUR_H
