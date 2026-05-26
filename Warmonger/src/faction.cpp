#include "Faction.hpp"
#include <iostream>
#include <stdexcept>



Faction::Faction(std::string name , int unitnum  , int attackp , int healthp , int unitreg)
:name{name} , 
attackPoint{attackp},
healthPoint{healthp},
unitRegNum{unitreg}
{
    numberOfUnits = unitnum;
    isLiving       = true;
}


void Faction::AssignEnemies(Faction* enemy1 , Faction* enemy2)
{
    if(enemy1->getType() == enemy2->getType()) throw std::invalid_argument("Enemy factions must be different.");
    
    firstEnemy  = enemy1;
    secondEnemy = enemy2;
}


void Faction::Print() const 
{
    std::cout << "Faction Name: " << name << '\n'
              << "Status: " << (IsAlive() ? "Alive" : "Defeated") << '\n'
              << "Number of Units: " << numberOfUnits << '\n'
              << "Attack Point: " << attackPoint << '\n'
              << "Health Point: " << healthPoint << '\n'
              << "Unit Regen Number: " << unitRegNum << '\n'
              << "Total Faction Health: " << getTotalHealth() << std::endl;
}



void Faction::EndTurn()
{
    if(numberOfUnits < 0 ) numberOfUnits = 0;
    if(numberOfUnits == 0) isLiving = false;
}



void Faction::setUnitNum(int damage)
{
    numberOfUnits -= damage/healthPoint;
}
