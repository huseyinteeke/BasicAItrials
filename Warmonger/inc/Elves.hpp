#pragma once
#include "Faction.hpp"

class Elves: public Faction
{
    
    public:
        Elves(std::string name = "NoName" , int unitnum = 50 , int attackp = 30 , int healthp = 150 , int unitreg = 10):
            Faction(name , unitnum , attackp , healthp , unitreg){}

        void PerformAttack() const override;
        void ReceiveAttack(std::string& type , int totaldamage) override;
        int PurchaseWeapons(int amount) override;
        int PurchaseArmors(int amount) override;
        void Print() const;
};
