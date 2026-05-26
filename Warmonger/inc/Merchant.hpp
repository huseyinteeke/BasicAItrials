#pragma once
#include "Faction.hpp"
#include <string>

class Merchant
{
    private:
        Faction* firstFaction;
        Faction* secondFaction;
        Faction* thirdFaction;
        int startingWeaponpoint;
        int startingArmorpoint;
        int revenue;
        int weaponLeftfortheDay;
        int armorLeftfortheDay;
    
        Faction* GetFactionByName(const std::string& name);
    
    public:
        Merchant(int startingWeaponpoint = 10 , int startingArmorpoint = 10):
        startingWeaponpoint(startingWeaponpoint),startingArmorpoint(startingArmorpoint) ,  revenue{0}, weaponLeftfortheDay{startingWeaponpoint}, armorLeftfortheDay{startingArmorpoint}
        {}

        void AssignFactions(Faction* fac1 , Faction* fac2 , Faction* fac3)
        {firstFaction = fac1; secondFaction = fac2; thirdFaction = fac3;}

        int GetWeaponPoints() const{return weaponLeftfortheDay;}
        int GetArmorPoints() const{return armorLeftfortheDay;}
        int GetRevenue() const{return revenue;}

        bool SellWeapons(const std::string& buyer , int amount);
        bool SellArmors(const std::string& buyer , int amount);
        void EndTurn();

    };