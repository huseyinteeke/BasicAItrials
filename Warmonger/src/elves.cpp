
#include "Elves.hpp"
#include <iostream>



void Elves::PerformAttack() const
{
    int totalDamage = getTotalDamage(); 
    int orcDamage  = static_cast<int>(0.6 * totalDamage);
    int dwarveDamage  = static_cast<int>(0.4 * totalDamage);

    std::string type = getType();
    if (Faction::getEnemy1()->IsAlive() 
        && Faction::getEnemy2()->IsAlive())
    {
        if(Faction::getEnemy1()->getType() == "Orcs"){
            Faction::getEnemy1()->ReceiveAttack(type , orcDamage); 
            Faction::getEnemy2()->ReceiveAttack(type , dwarveDamage * 1.5); 
        }else
        {
            Faction::getEnemy1()->ReceiveAttack(type , dwarveDamage * 1.5); 
            Faction::getEnemy2()->ReceiveAttack(type , orcDamage); 
        }
    }
    else if(Faction::getEnemy1()->IsAlive() 
        || Faction::getEnemy2()->IsAlive())
    {
        if(Faction::getEnemy1()->IsAlive()){
            if(Faction::getEnemy1()->getType() == "Dwarves") totalDamage *= 1.5;
            Faction::getEnemy1()->ReceiveAttack(type , totalDamage); 
        }
        else{
                if(Faction::getEnemy2()->getType() == "Dwarves") totalDamage *= 1.5;
                Faction::getEnemy2()->ReceiveAttack(type , totalDamage); 
        }
    }
}


void Elves::ReceiveAttack(std::string& type , int totalDamage) 
{
    float damageTaken;
    if(type == "Orcs")
    {
        damageTaken = totalDamage * 1.25;
    }
    else
    {
        damageTaken = totalDamage * 0.75;
    }
    setUnitNum(static_cast<int>(damageTaken));
}


void Elves::Print() const
{
    Faction::Print();
    std::cout <<"You cannot reach our elegance."<<std::endl;
}


int Elves::PurchaseWeapons(int amount)
{
    setAttackPoint(amount * 2);
    return amount * 15;
}



int Elves::PurchaseArmors(int amount)
{
    setHealthPoint(amount * 4);
    return  amount * 5;
}