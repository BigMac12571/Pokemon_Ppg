#include "pokemondata.h"
QMap<QPair<int, int>, PokemonData::PokemonBaseData> PokemonData::baseDataMap;
PokemonData::PokemonData(int id, int level):id_(id),level_(level)
{
    if (baseDataMap.isEmpty()) {
            InitializeBaseData();
        }

    UpdateData();
    current_hp = max_hp;


}
void PokemonData::InitializeBaseData()
{
    baseDataMap[qMakePair(0, 1)] = {"Bulbasaur", 5, 5, 30, "Tackle", "Growl", 10, 15, 20,20, ":/new/prefix1/Dataset/Image/battle/bulbasaur.png",":/new/prefix1/Dataset/Image/battle/bulbasaur_back.png"};
    baseDataMap[qMakePair(0, 3)] = {"Ivysaur", 10, 10, 80, "Growth", "Razor Leaf", 20, 25, 15, 5, ":/new/prefix1/Dataset/Image/battle/ivysaur.png",":/new/prefix1/Dataset/Image/battle/ivysaur_back.png"};
    baseDataMap[qMakePair(0, 5)] = {"Venusaur", 15, 15, 100, "x", "x", 0, 0, 0,0, ":/new/prefix1/Dataset/Image/battle/venusaur.png",":/new/prefix1/Dataset/Image/battle/venusaur_back.png"};

    baseDataMap[qMakePair(1, 1)] = {"Squirtle", 5, 5, 30, "Tackle", "Tail Whip", 10, 15, 20,20, ":/new/prefix1/Dataset/Image/battle/squirtle.png",":/new/prefix1/Dataset/Image/battle/squirtle_back.png"};
    baseDataMap[qMakePair(1, 3)] = {"Wartortle", 10, 10, 80, "Protect", "Wave Crash", 20, 25, 15, 5, ":/new/prefix1/Dataset/Image/battle/wartortle.png",":/new/prefix1/Dataset/Image/battle/wartortle_back.png"};
    baseDataMap[qMakePair(1, 5)] = {"Blastoise", 15, 15, 100, "x", "x", 0, 0, 0,0, ":/new/prefix1/Dataset/Image/battle/blastoise.png",":/new/prefix1/Dataset/Image/battle/blastoise_back.png"};

    baseDataMap[qMakePair(2, 1)] = {"Charmander", 5, 5, 30, "Scratch", "Growl", 10, 15, 20,20, ":/new/prefix1/Dataset/Image/battle/charmander.png",":/new/prefix1/Dataset/Image/battle/charmander_back.png"};
    baseDataMap[qMakePair(2, 3)] = {"Charmeleon", 10, 10, 80, "Scary face", "Flare Blitz", 20, 25, 15, 5, ":/new/prefix1/Dataset/Image/battle/charmeleon.png",":/new/prefix1/Dataset/Image/battle/charmeleon_back.png"};
    baseDataMap[qMakePair(2, 5)] = {"Charizard", 15, 15, 100, "x", "x", 0, 0, 0,0, ":/new/prefix1/Dataset/Image/battle/charizard.png",":/new/prefix1/Dataset/Image/battle/charizard_back.png"};
}
void PokemonData::LevelUp()
{
    level_++;
    auto key = qMakePair(id_, level_);
       if (baseDataMap.contains(key)) {
           const PokemonBaseData& data = baseDataMap[key];
           name = data.name;
           attack = data.attack;
           defense = data.defense;
           max_hp = data.max_hp;
           type1 = data.type1;
           type2 = data.type2;
           power1 = data.power1;
           power2 = data.power2;
           pp1 = data.pp1;
           pp2 = data.pp2;
           ImagePath = data.imagePath;
           BackImagePath = data.BackImagePath;
       }
}

void PokemonData::UpdateData()
{
    int dataLevel;

    if (level_ <= 2) {
        dataLevel = 1;
    } else if (level_ <= 4) {
        dataLevel = 3;
    } else {
        dataLevel = 5;
    }
    auto key = qMakePair(id_, level_);
    if (baseDataMap.contains(key)) {
        const PokemonBaseData& data = baseDataMap[key];
        name = data.name;
        attack = data.attack;
        defense = data.defense;
        max_hp = data.max_hp;
        type1 = data.type1;
        type2 = data.type2;
        power1 = data.power1;
        power2 = data.power2;
        pp1 = data.pp1;
        pp2 = data.pp2;
        ImagePath = data.imagePath;
        BackImagePath = data.BackImagePath;
        }
}


int PokemonData::GetDamage(const PokemonData& opponent, int MoveIndex) const
{
    int selectedPower = 0;
    if (MoveIndex == 1) {
        selectedPower = power1;
    } else if (MoveIndex == 2) {
        selectedPower = power2;
    } else {
        selectedPower = 0;  // 預設是普通攻擊
    }

    int damage = (selectedPower + attack - opponent.defense) * level_;
    return damage < 0 ? 0 : damage;
}
void PokemonData::TakeDamage(int damage)
{
    current_hp -= damage;
    if (current_hp < 0) {
        current_hp = 0;
    }
}
