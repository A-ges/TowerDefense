#ifndef TOWER_H
#define TOWER_H
#include <iostream>
using namespace std;

//This file declares the tower class

class Tower{
private:        //declare all private values for tower
    int row;
    int col;
    int damage;
    int range;
public:
    Tower(int r, int c); //constructor with position

    //getters
    int getRow() const; //getters, like with castle, const because I want to prevent any changes to variables
    int getCol() const;
    int getDamage() const; //could change due to updates by the player
    int getRange() const;

    //functions
    void upgrade();  //called to upgrade a tower (+1 damage)
    bool isInRange(int enemyRow, int enemyCol) const; //checks if enemy is in range of tower on which it is called
};

#endif
