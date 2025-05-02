#include "pokemondata.h"
QMap<QPair<int, int>, PokemonData::PokemonBaseData> PokemonData::baseDataMap;
PokemonData::PokemonData(int id, int level):id_(id),level_(level)
{
    if (baseDataMap.isEmpty()) {
            InitializeBaseData();
        }

    UpdateData();
    current_hp = max_hp;
    current_pp = {pp0,pp1,pp2,pp3};


}
void PokemonData::InitializeBaseData()
{
    baseDataMap[qMakePair(-1, 1)] = {"", 0, 0, 0, "", "","","", 0, 0,0,0, 0,0,0,0,-1, "",""};


    baseDataMap[qMakePair(0, 1)] = {"Bulbasaur", 5, 5, 30, "Tackle", "x","x","x", 10, 0,0,0, 20,0,0,0, 0,":/new/prefix1/Dataset/Image/battle/bulbasaur.png",":/new/prefix1/Dataset/Image/battle/bulbasaur_back.png"};
    baseDataMap[qMakePair(0, 2)] = {"Bulbasaur", 5, 5, 30, "Tackle", "Growl","x","x", 10, 15,0,0, 20,20,0,0, 0,":/new/prefix1/Dataset/Image/battle/bulbasaur.png",":/new/prefix1/Dataset/Image/battle/bulbasaur_back.png"};
    baseDataMap[qMakePair(0, 3)] = {"Ivysaur", 10, 10, 80, "Tackle", "Growl", "Growth", "x",10, 20, 25,0, 20,20,15, 0, 0, ":/new/prefix1/Dataset/Image/battle/ivysaur.png",":/new/prefix1/Dataset/Image/battle/ivysaur_back.png"};
    baseDataMap[qMakePair(0, 4)] = {"Ivysaur", 10, 10, 80, "Tackle", "Growl", "Growth", "Razor Leaf", 10, 15, 20, 25, 20,20,15, 5, 0, ":/new/prefix1/Dataset/Image/battle/ivysaur.png",":/new/prefix1/Dataset/Image/battle/ivysaur_back.png"};
    baseDataMap[qMakePair(0, 5)] = {"Venusaur", 15, 15, 100, "Tackle", "Growl", "Growth", "Razor Leaf", 10, 15, 20, 25, 20,20,15, 5, 0, ":/new/prefix1/Dataset/Image/battle/venusaur.png",":/new/prefix1/Dataset/Image/battle/venusaur_back.png"};

    baseDataMap[qMakePair(1, 1)] = {"Squirtle", 5, 5, 30, "Tackle", "x", "x", "x", 10, 0,0,0, 20,0,0,0, 1, ":/new/prefix1/Dataset/Image/battle/squirtle.png",":/new/prefix1/Dataset/Image/battle/squirtle_back.png"};
    baseDataMap[qMakePair(1, 2)] = {"Squirtle", 5, 5, 30, "Tackle", "Tail Whip", "x", "x", 10, 15,0,0, 20,20,0,0,1, ":/new/prefix1/Dataset/Image/battle/squirtle.png",":/new/prefix1/Dataset/Image/battle/squirtle_back.png"};
    baseDataMap[qMakePair(1, 3)] = {"Wartortle", 10, 10, 80, "Tackle", "Tail Whip", "Protect", "x",10, 20, 25,0, 20,20,15, 0,1, ":/new/prefix1/Dataset/Image/battle/wartortle.png",":/new/prefix1/Dataset/Image/battle/wartortle_back.png"};
    baseDataMap[qMakePair(1, 4)] = {"Wartortle", 10, 10, 80, "Tackle", "Tail Whip", "Protect", "Wave Crash", 10, 15, 20, 25, 20,20,15, 5,1, ":/new/prefix1/Dataset/Image/battle/wartortle.png",":/new/prefix1/Dataset/Image/battle/wartortle_back.png"};
    baseDataMap[qMakePair(1, 5)] = {"Blastoise", 15, 15, 100, "Tackle", "Tail Whip", "Protect", "Wave Crash", 10, 15, 20, 25, 20,20,15, 5,1, ":/new/prefix1/Dataset/Image/battle/blastoise.png",":/new/prefix1/Dataset/Image/battle/blastoise_back.png"};

    baseDataMap[qMakePair(2, 1)] = {"Charmander", 5, 5, 30, "Scratch", "x", "x", "x", 10, 0,0,0, 20,0,0,0, 2,":/new/prefix1/Dataset/Image/battle/charmander.png",":/new/prefix1/Dataset/Image/battle/charmander_back.png"};
    baseDataMap[qMakePair(2, 2)] = {"Charmander", 5, 5, 30, "Scratch", "Growl", "x", "x", 10, 15,0,0, 20,20,0,0,2, ":/new/prefix1/Dataset/Image/battle/charmander.png",":/new/prefix1/Dataset/Image/battle/charmander_back.png"};
    baseDataMap[qMakePair(2, 3)] = {"Charmeleon", 10, 10, 80, "Scratch", "Growl", "Scary face", "x",10, 20, 25,0, 20,20,15, 0,2, ":/new/prefix1/Dataset/Image/battle/charmeleon.png",":/new/prefix1/Dataset/Image/battle/charmeleon_back.png"};
    baseDataMap[qMakePair(2, 4)] = {"Charmeleon", 10, 10, 80, "Scratch", "Growl", "Scary face", "Flare Blitz", 10, 15, 20, 25, 20,20,15, 5, 2,":/new/prefix1/Dataset/Image/battle/charmeleon.png",":/new/prefix1/Dataset/Image/battle/charmeleon_back.png"};
    baseDataMap[qMakePair(2, 5)] = {"Charizard", 15, 15, 100, "Scratch", "Growl", "Scary face", "Flare Blitz", 10, 15, 20, 25, 20,20,15, 5, 2,":/new/prefix1/Dataset/Image/battle/charizard.png",":/new/prefix1/Dataset/Image/battle/charizard_back.png"};
}
void PokemonData::LevelUp()
{   int before_hp = GetCurrentHp();
    int before_max_hp = GetMaxHp();

    level_++;
    UpdateData();
    current_hp = before_hp +(GetMaxHp()-before_max_hp);
    current_pp = {GetCurrentPP(0),(level_==2)? pp1: GetCurrentPP(1), (level_==3)?pp2:GetCurrentPP(2),(level_==4)?pp3:GetCurrentPP(3)};

}

void PokemonData::UpdateData()
{/*
    int dataLevel;

    if (level_ <= 2) {
        dataLevel = 1;
    } else if (level_ <= 4) {
        dataLevel = 3;
    } else {
        dataLevel = 5;
    }*/
    auto key = qMakePair(id_, level_);
    if (baseDataMap.contains(key)) {
        const PokemonBaseData& data = baseDataMap[key];
        name = data.name;
        attack = data.attack;
        defense = data.defense;
        max_hp = data.max_hp;
        move0 = data.move0;
        move1 = data.move1;
        move2 = data.move2;
        move3 = data.move3;
        power0 = data.power0;
        power1 = data.power1;
        power2 = data.power2;
        power3 = data.power3;
        pp0 = data.pp0;
        pp1 = data.pp1;
        pp2 = data.pp2;
        pp3 = data.pp3;
        type = data.type;
        ImagePath = data.imagePath;
        BackImagePath = data.BackImagePath;
        }
}


int PokemonData::GetDamage(const PokemonData& opponent, int move) const
{
    int selectedPower = 0;
    selectedPower = (move ==0)? power0: (move ==1)? power1:(move ==2)? power2:(move ==3)? power3: 0;

    double effectiveness = 1 ;
    if((GetType()+1)%3 == opponent.GetType()){
        effectiveness = 2 ;
    }
    if( GetType() == (opponent.GetType() +1)%3){
        effectiveness = 0.5 ;
    }

    int damage = (selectedPower + (attack * effectiveness) - opponent.defense) * level_ ;

    if (damage < 0) damage = 0;

    return damage;
}
void PokemonData::TakeDamage(int damage)
{
    current_hp -= damage;
    if (current_hp < 0) {
        current_hp = 0;
    }
}

void PokemonData::RestoreHP(){
    current_hp += 10;
    if (current_hp > GetMaxHp()) {
        current_hp = GetMaxHp();
    }

}

void PokemonData::UseMove(int MoveID){
    if(current_pp[MoveID]>0){
        current_pp[MoveID] -- ;
     }
}
void PokemonData::RestorePP(int MoveID){

    if(current_pp[MoveID]<GetMaxPP(MoveID)){
        current_pp[MoveID] = GetMaxPP(MoveID) ;
     }


}
void PokemonData::Reset()
{
    current_hp = max_hp;

    const auto key = qMakePair(0, 1);
    if (baseDataMap.contains(key)) {
        const auto& base = baseDataMap[key];
        pp1 = base.pp1;
        pp2 = base.pp2;
    } else {
        qDebug() << "Reset failed: No base data for ID" << id_ << "Level" << level_;
    }
}
