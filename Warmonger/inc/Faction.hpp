#pragma once
#include <string>

class Faction
{
    private:
        std::string name;
        Faction* firstEnemy; 
        Faction* secondEnemy;
        int numberOfUnits {};
        int attackPoint;
        int healthPoint;
        int unitRegNum;
        bool isLiving;

    protected:
        
        int  getTotalDamage() const {return (numberOfUnits * attackPoint);}
        void setUnitNum(int damage);
        void setAttackPoint(int amount){attackPoint += amount;}
        void setHealthPoint(int amount){healthPoint += amount;}
    public:
        virtual ~Faction() = default;

        std::string getType() const {return name;}
        bool IsAlive() const {return isLiving;}
        int getNumberOfUnits() const { return numberOfUnits; }
        int getAttackPoint() const { return attackPoint; }
        int getHealthPoint() const { return healthPoint; }
        int getUnitRegNum() const { return unitRegNum; }
        Faction(std::string name = "NoName" , int unitnum = 50 , int attackp = 30 , int healthp = 150 , int unitreg = 10);
        void AssignEnemies(Faction* enemy1 , Faction* enemy2);
        virtual void PerformAttack() const = 0;
        virtual void ReceiveAttack(std::string& type , int totaldamage) = 0;
        virtual int  PurchaseWeapons(int amount) = 0;
        virtual int PurchaseArmors(int amount) = 0;
        virtual void Print() const;
        void EndTurn();

        Faction* getEnemy1() const{return firstEnemy;}
        Faction* getEnemy2() const{return secondEnemy;}
        int getTotalHealth() const { return numberOfUnits * healthPoint; }
};