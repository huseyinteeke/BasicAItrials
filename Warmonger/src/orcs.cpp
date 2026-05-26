#include "Orcs.hpp"
#include <iostream>



void Orcs::PerformAttack() const
{
    int totalDamage = getTotalDamage();
    
    int elveDamage  = static_cast<int>(0.7 * totalDamage);
    int dwarveDamage  = static_cast<int>(0.3 * totalDamage);

    std::string type = getType();
    if (Faction::getEnemy1()->IsAlive() 
        && Faction::getEnemy2()->IsAlive())
    {
        if(Faction::getEnemy1()->getType() == "Elves"){
            Faction::getEnemy1()->ReceiveAttack(type , elveDamage); 
            Faction::getEnemy2()->ReceiveAttack(type , dwarveDamage); 
        }else
        {
            Faction::getEnemy1()->ReceiveAttack(type , dwarveDamage); 
            Faction::getEnemy2()->ReceiveAttack(type , elveDamage); 
        }
    }
    else if(Faction::getEnemy1()->IsAlive() 
        || Faction::getEnemy2()->IsAlive())
    {
        Faction::getEnemy1()->IsAlive() ? Faction::getEnemy1()->ReceiveAttack(type , totalDamage) : 
        Faction::getEnemy2()->ReceiveAttack(type , totalDamage); 
    }

}


void Orcs::ReceiveAttack(std::string& type , int totalDamage) 
{
    float damageTaken = 0; 
    if(type == "Elves")
    {  
       damageTaken = 0.75 * totalDamage; 
    }
    else
    {
        damageTaken = 0.8 * totalDamage; 
    }

    setUnitNum(static_cast<int>(damageTaken));

}


void Orcs::Print() const
{
    Faction::Print();
    std::cout <<"Stop running , you'll only die tired."<<std::endl;
}



int Orcs::PurchaseWeapons(int amount)
{
    setAttackPoint(amount * 2);
    return amount * 20;
}



int Orcs::PurchaseArmors(int amount)
{
    setHealthPoint(amount * 3);
    return  amount * 1;
}