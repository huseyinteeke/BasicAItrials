#include "Merchant.hpp"
#include <iostream>

using namespace std;

Faction* Merchant::GetFactionByName(const std::string& name)
{
    if(name == firstFaction->getType()) return firstFaction;
    if(name == secondFaction->getType()) return secondFaction;
    if(name == thirdFaction->getType()) return thirdFaction;
    return nullptr;
}

bool Merchant::SellWeapons(const std::string& buyer , int amount)
{

    Faction* tmp = GetFactionByName(buyer);
    if(tmp == nullptr) return false;

    if(!tmp->IsAlive())
    {
        cout<<"The faction you want to sell weapons is dead!"<<endl;
        return false;
    }


    if(amount > weaponLeftfortheDay)
    {
        cout<<"You try to sell more weapons than you have in possession!"<<endl;
        return false;
    }

    cout<<"Weapons sold"<<endl;
    revenue += tmp->PurchaseWeapons(amount);
    weaponLeftfortheDay -= amount;
    return true;

}





bool Merchant::SellArmors(const std::string& buyer , int amount)
{
    Faction* tmp = GetFactionByName(buyer);
    if(tmp == nullptr) return false;

    if(!tmp->IsAlive())
    {
        cout<<"The faction you want to sell armor is dead!"<<endl;
        return false;
    }


    if(amount > armorLeftfortheDay)
    {
        cout<<"You try to sell more armors than you have in possession!"<<endl;
        return false;
    }

    cout<<"Armors sold"<<endl;
    revenue += tmp->PurchaseArmors(amount);
    armorLeftfortheDay -= amount;
    return true;

}


void Merchant::EndTurn()
{   
    weaponLeftfortheDay = startingWeaponpoint;
    armorLeftfortheDay  = startingArmorpoint;
}