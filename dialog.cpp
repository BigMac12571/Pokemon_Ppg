#include "dialog.h"
Dialog::Dialog(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(525, 99);
    this->move(5, View_Height - 99);

    Oak_dialog << "I am Professor Oak. Welcome to my laboratory!"
               << "You can choose one from three Poké Balls as your initial\nPokémon in Laboratory."
               << "這是第三段對話。"
               << "這是第四段對話。";

    CurrentDialog = 0;
    setText(Oak_dialog.at(CurrentDialog));  // 顯示第一段對話

    setAlignment(Qt::AlignCenter);  // 文字居中顯示
    setStyleSheet("color: black; font-size: 25px;");
}
void Dialog::Oak_Dialog()
{
    if (CurrentDialog < Oak_dialog.size() - 1) {
          // 增加對話索引
        setText(Oak_dialog.at(CurrentDialog));  // 更新顯示的文字
        CurrentDialog++;
    } else {
        // 選擇性關閉對話框
        close();  // 如果希望在對話結束後自動關閉窗口
        CurrentDialog = 0;
    }
}

void Dialog::paintEvent(QPaintEvent *event)
{
    // 確保在 QLabel 顯示文字時仍然保持圖片顯示
    QPainter PaintDialog(this);
    QPixmap pixmap(":/new/prefix1/Dataset/Image/dialog.png");
    // 繪製背景圖片
    PaintDialog.drawPixmap(0, 0, width(), height(), pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel::paintEvent(event); // 顯示你設定的文字（像你用 setText() 設定的句子）

}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    // 如果按下的是 A 鍵，顯示下一段對話
    if (event->key() == Qt::Key_A) {
        Oak_Dialog();
    }
}
