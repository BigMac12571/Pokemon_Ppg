#include "dialog.h"
Dialog::Dialog(QWidget *parent) : QLabel(parent)
{
    this->setFixedSize(525, 99);
    this->move(0, View_Height - 99);

    Oak_dialog << "I am Professor Oak. Welcome to my laboratory!"
               << "You can choose one from three Poké Balls as your initial\nPokémon in Laboratory.";

    Sign_dialog <<"This is Pallet Town. Begin your adventure!";

    CurrentDialog = 0;
    setText(Oak_dialog.at(CurrentDialog));  // 顯示第一段對話

    setAlignment(Qt::AlignCenter);  // 文字居中顯示
    setStyleSheet("color: black; font-size: 20px;");


}
void Dialog::Oak_Dialog(){
    if (CurrentDialog < Oak_dialog.size()) {

        setText(Oak_dialog.at(CurrentDialog));
        CurrentDialog++;
    } else {
        emit Close_Dialog();
        CurrentDialog = 0;
    }
}

void Dialog::Sign_Dialog(){
    if (CurrentDialog < Sign_dialog.size()) {

        setText(Sign_dialog.at(CurrentDialog));
        CurrentDialog++;
    } else {
        emit Close_Dialog();
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
