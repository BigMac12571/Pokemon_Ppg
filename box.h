#ifndef BOX_H
#define BOX_H

#include <QLabel>
#include <QWidget>

// 簡單的箱子：用來顯示圖片、記錄是否已撿取
class Box : public QLabel {


public:
    explicit Box(QWidget *parent = nullptr);


};

#endif // BOX_H


