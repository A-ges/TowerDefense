#include "Tower.h"
#include <iostream>
#include <cmath> //for abs function
//This file implements the tower class
using namespace std;


Tower::Tower(int r, int c){ //constructor, needs row and column
    row = r;
    col = c;
    damage = 1;
    range = 2;
}
//simple getters
int Tower::getRow() const{
    return row;
}

int Tower::getCol() const{
    return col;
}

int Tower::getDamage() const{
    return damage;
}

int Tower::getRange() const{
    return range;
}

//functions
void Tower::upgrade(){
    damage++;       //all checks are done in the game.cpp file, here i just add damage and range
    range++;
}

bool Tower::isInRange(int enemyRow, int enemyCol) const{  //formula adopted from the task description on notion
    int rowDiff = abs(row - enemyRow);
    int colDiff = abs(col - enemyCol);

    return (rowDiff <= range && colDiff <= range);
}
