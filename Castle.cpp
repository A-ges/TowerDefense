#include "Castle.h"
#include <iostream>
using namespace std;

//This file implements the castle class
//castle is always at a set place with set health so construct it there
Castle::Castle(){
    row = 19;
    col = 9;
    health = 100;
}


//getters
int Castle::getRow() const{
    return row;
}

int Castle::getCol() const{
    return col;
}

int Castle::getHealth() const{
    return health;
}

//functions
void Castle::takeDamage(int dmg){
    health -= dmg; //subtract enemy damage of off the castle health
    if(health < 0){
        health = 0; //to make sure there's no negative health
    }
}

bool Castle::isDestroyed() const{
    return health <= 0;   //true if <=0 health
}
