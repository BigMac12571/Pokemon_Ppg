#include "pokemondata.h"
QMap<QPair<int, int>, PokemonData::PokemonBaseData> PokemonData::baseDataMap;
PokemonData::PokemonData(int id, int form):id_(id),form_(form)
{
    if (baseDataMap.isEmpty()) {
            InitializeBaseData();
        }

        auto key = qMakePair(id_, form_);
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

        level = 1; // 預設等級



}
void PokemonData::InitializeBaseData()
{
    baseDataMap[qMakePair(0, 0)] = {"Bulbasaur", 5, 5, 30, "Tackle", "Growl", 10, 15, 20,20, ":/new/prefix1/Dataset/Image/battle/bulbasaur.png",":/new/prefix1/Dataset/Image/battle/bulbasaur_back.png"};
    baseDataMap[qMakePair(0, 1)] = {"Ivysaur", 10, 10, 80, "Growth", "Razor Leaf", 20, 25, 15, 5, ":/new/prefix1/Dataset/Image/battle/ivysaur.png",":/new/prefix1/Dataset/Image/battle/ivysaur_back.png"};
    baseDataMap[qMakePair(0, 2)] = {"Venusaur", 15, 15, 100, "x", "x", 0, 0, 0,0, ":/new/prefix1/Dataset/Image/battle/venusaur.png",":/new/prefix1/Dataset/Image/battle/venusaur_back.png"};

    baseDataMap[qMakePair(1, 0)] = {"Squirtle", 5, 5, 30, "Tackle", "Tail Whip", 10, 15, 20,20, ":/new/prefix1/Dataset/Image/battle/squirtle.png",":/new/prefix1/Dataset/Image/battle/squirtle_back.png"};
    baseDataMap[qMakePair(1, 1)] = {"Wartortle", 10, 10, 80, "Protect", "Wave Crash", 20, 25, 15, 5, ":/new/prefix1/Dataset/Image/battle/wartortle.png",":/new/prefix1/Dataset/Image/battle/wartortle_back.png"};
    baseDataMap[qMakePair(1, 2)] = {"Blastoise", 15, 15, 100, "x", "x", 0, 0, 0,0, ":/new/prefix1/Dataset/Image/battle/blastoise.png",":/new/prefix1/Dataset/Image/battle/blastoise_back.png"};

    baseDataMap[qMakePair(2, 0)] = {"Charmander", 5, 5, 30, "Scratch", "Growl", 10, 15, 20,20, ":/new/prefix1/Dataset/Image/battle/charmander.png",":/new/prefix1/Dataset/Image/battle/charmander_back.png"};
    baseDataMap[qMakePair(2, 1)] = {"Charmeleon", 10, 10, 80, "Scary face", "Flare Blitz", 20, 25, 15, 5, ":/new/prefix1/Dataset/Image/battle/charmeleon.png",":/new/prefix1/Dataset/Image/battle/charmeleon_back.png"};
    baseDataMap[qMakePair(2, 2)] = {"Charizard", 15, 15, 100, "x", "x", 0, 0, 0,0, ":/new/prefix1/Dataset/Image/battle/charizard.png",":/new/prefix1/Dataset/Image/battle/charizard_back.png"};
}
void PokemonData::LevelUp()
{
    level++;
    UpdateForm();
}

void PokemonData::UpdateForm()
{
    int newForm = form_;

    if (level <= 2) {
        newForm = 0;
    } else if (level <= 4) {
        newForm = 1;
    } else if (level <= 6) {
        newForm = 2;
    }

    if (newForm != form_) {
        form_ = newForm;

        // 更新成新的資料
        auto key = qMakePair(id_, form_);
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
}

int PokemonData::GetDamage(int movePower, int opponentDefense) const
{
    int damage = (movePower + attack - opponentDefense) * level;
    if (damage < 0) damage = 0; // 傷害不能負的
    return damage;
}
