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
    PokemonData() : id_(0), level_(1) {}
    PokemonData(int id, int level);
    void LevelUp();

    // Getter
    QString GetName() const { return name; }
    QString GetType1() const { return type1; }
    QString GetType2() const { return type2; }
    int GetAttack() const { return attack; }
    int GetDefense() const { return defense; }
    int GetCurrentHp() const { return current_hp;}
    int GetMaxHp() const { return max_hp; }
    int GetLevel() const { return level_; }
    QString GetImagePath() const { return ImagePath; }
    QString GetBackImagaePath() const { return BackImagePath;}
    int GetDamage(const PokemonData& opponent, int MoveIndex) const;
    void TakeDamage(int damage);



private:

    int id_;
    int level_;
    int attack;
    int defense;
    int max_hp;
    int current_hp;
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
    static QMap<QPair<int, int>, PokemonBaseData> baseDataMap;  // (id, level) -> 資料
    static void InitializeBaseData(); // 初始設定

    void UpdateData(); // 自動改變form
};

#endif // POKEMONDATA_H
