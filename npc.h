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


class NPC: public QLabel
{
    Q_OBJECT

public:

    explicit NPC(QWidget *parent = nullptr);

private:

};

#endif // TITLESCREEN_H
