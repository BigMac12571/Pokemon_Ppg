#ifndef POKEMONDATA_H
#define POKEMONDATA_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QMap>
#include <QRandomGenerator>
#include <QDebug>

class PokemonData
{
public:
    PokemonData() : id_(0), form_(0) {}
    PokemonData(int id, int form);
    void LevelUp();

    // Getter
      QString GetName() const { return name; }
      QString GetType1() const { return type1; }
      QString GetType2() const { return type2; }
      int GetAttack() const { return attack; }
      int GetDefense() const { return defense; }
      int GetMaxHp() const { return max_hp; }
      int GetLevel() const { return level; }
      QString GetImagePath() const { return ImagePath; }
      QString GetBackImagaePath() const { return BackImagePath;}
      int GetDamage(int movePower, int opponentDefense) const;




private:

    int id_;
    int form_;
    int level;
    int attack;
    int defense;
    int max_hp;
    QString name;
    QString type1;
    QString type2;
    int power1;
    int power2;
    int pp1;
    int pp2;
    QString ImagePath;
    QString BackImagePath;

    struct PokemonBaseData {
        QString name;
        int attack;
        int defense;
        int max_hp;
        QString type1;
        QString type2;
        int power1;
        int power2;
        int pp1;
        int pp2;
        QString imagePath;
        QString BackImagePath;
        };
    static QMap<QPair<int, int>, PokemonBaseData> baseDataMap;  // (id, form) -> 資料
    static void InitializeBaseData(); // 初始設定

    void UpdateForm(); // 自動改變form
};

#endif // POKEMONDATA_H
