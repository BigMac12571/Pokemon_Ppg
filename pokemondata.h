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
    PokemonData() : id_(-1), level_(1) {}
    PokemonData(int id, int level);
    void LevelUp();

    // Getter
    int GetID() const { return id_ ; }
    QString GetName() const { return name; }
    QString GetMove(int move) const { return (move == 0)? move0: (move == 1)? move1: (move == 2)? move2: (move == 3)? move3: "WTF"; }
    QString GetSpecialATK(int spec) const { return Special_Attack.at(spec);}
    QList<int> GetSpecialatk (int spec) const { return special_atk.at(spec);} //0=>damage 1=> attack times 2=> use times
    int GetCurrentSpecialatk_remaing_times (int spec) const { return current_remaing_special_atk.at(spec);}
    int GetAttack() const { return attack; }
    int GetDefense() const { return defense; }
    int GetCurrentHp() const { return current_hp;}

    int GetMaxHp() const { return max_hp; }
    int GetLevel() const { return level_; }
    int GetType() const { return type; }
    int GetCurrentPP(int pp) const { return  (pp == 0)? current_pp.at(0): (pp == 1)? current_pp.at(1): (pp == 2)? current_pp.at(2): (pp == 3)? current_pp.at(3): 0;}
    int GetMaxPP(int pp) const { return  (pp == 0)? pp0: (pp == 1)? pp1: (pp == 2)? pp2: (pp == 3)? pp3: 0;}

    double GetSA() const { return SA;}
    double GetDA() const { return SD;}
    int GetSpeed() const { return speed;}

    QString GetImagePath() const { return ImagePath; }
    QString GetBackImagaePath() const { return BackImagePath;}
    int GetDamage(const PokemonData& opponent, int move, int special = -1) const;
    void TakeDamage(int damage);
    void RestoreHP();
    void RestorePP(int MoveID);
    void UseMove(int MoveID);
    void UseSpec(int SpecID);
    void SetSpec();
    bool isEmpty() const { return id_ == -1; }

    void SetBagLocation(int loc) { Bag_Location = loc; }
    int GetBagLocation() const { return Bag_Location; }




private:

    int id_;
    int level_;
    int attack;
    int defense;

    int max_hp;
    int current_hp;
    QString name;
    QString move0;
    QString move1;
    QString move2;
    QString move3;

    int power0;
    int power1;
    int power2;
    int power3;
    QVector<int> current_pp;
    int pp0;
    int pp1;
    int pp2;
    int pp3;
    int type;
    QList<QString> Special_Attack;
    QVector<int> current_remaing_special_atk;
    QList<QList<int>> special_atk;
    double SA;
    double SD;
    int speed;
    QString ImagePath;
    QString BackImagePath;

    struct PokemonBaseData {
        QString name;
        int attack;
        int defense;
        int max_hp;
        QString move0;
        QString move1;
        QString move2;
        QString move3;
        int power0;
        int power1;
        int power2;
        int power3;
        int pp0;
        int pp1;
        int pp2;
        int pp3;
        int type;
        QList<QString> Special_Attack;
        QList<QList<int>> special_atk;
        double SA;
        double SD;
        int speed;
        QString imagePath;
        QString BackImagePath;
        };
    static QMap<QPair<int, int>, PokemonBaseData> baseDataMap;  // (id, level) -> 資料
    static void InitializeBaseData(); // 初始設定

    void UpdateData(); // 自動改變form

    int Bag_Location = -1;
};

#endif // POKEMONDATA_H
