#ifndef BOX_H
#define BOX_H

#include <QLabel>
#include <QWidget>

// 簡單的箱子：用來顯示圖片、記錄是否已撿取
class Box : public QLabel {


public:
    explicit Box(QWidget *parent = nullptr);

    // 檢查是否已被撿取
    bool isPicked() const;

    // 撿取箱子（隱藏顯示，標記狀態）
    void pick();

private:
    bool picked = false;  // 是否已被撿取
};

#endif // BOX_H


