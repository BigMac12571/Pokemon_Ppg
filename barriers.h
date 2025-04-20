#ifndef BARRIERS_H
#define BARRIERS_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>

class BarrierWidget :public QWidget
{
    Q_OBJECT

public:
    explicit BarrierWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setFixedSize(50, 50); // 預設大小
    }

    void setColor(const QColor &color) {
        this->color = color;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(color);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(0, 0, width()-1, height()-1);
    }

private:
    QColor color = Qt::red;
};

#endif // BARRIERWIDGET_H
