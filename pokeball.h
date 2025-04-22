#ifndef POKEBALL_H
#define POKEBALL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QVector>
#include <QGraphicsOpacityEffect>

class Pokeball:public QLabel
{   Q_OBJECT




public:
     Pokeball(int id, QWidget *parent = nullptr);
     const int ID;




     void Pickup_Pokeball(int id);
private:
     bool getpicked;







};

#endif // POKEBALL_H
