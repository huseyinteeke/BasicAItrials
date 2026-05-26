
#include "Dwarves.hpp"
#include <iostream>



void Dwarves::PerformAttack() const
{
    int totalDamage  = getTotalDamage();
    std::string type = getType();
    if (Faction::getEnemy1()->IsAlive() 
        && Faction::getEnemy2()->IsAlive())
    {
        Faction::getEnemy1()->ReceiveAttack(type , totalDamage / 2);
        Faction::getEnemy2()->ReceiveAttack(type , totalDamage / 2);
    }

    else if(Faction::getEnemy1()->IsAlive() 
        || Faction::getEnemy2()->IsAlive())
    {
        Faction::getEnemy1()->IsAlive() ? Faction::getEnemy1()->ReceiveAttack(type , totalDamage) : 
        Faction::getEnemy2()->ReceiveAttack(type , totalDamage); 
    }

}


void Dwarves::ReceiveAttack(std::string& type , int totalDamage) 
{
    setUnitNum(static_cast<int>(totalDamage));
}


void Dwarves::Print() const
{
    Faction::Print();
    std::cout <<"Taste the power of our axes!"<<std::endl;
}



int Dwarves::PurchaseWeapons(int amount)
{
    setAttackPoint(amount);
    return amount * 10;
}



int Dwarves::PurchaseArmors(int amount)
{
    setHealthPoint(amount);
    return  amount * 3;
}