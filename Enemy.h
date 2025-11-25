#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
using namespace std;
//This file declares the enemy class

class Enemy{
private:  //declare all private values for enemy
    int row;
    int col;
    int health;
    int speed;
    bool alive;

public:
    Enemy(int c, int h, int s); //constructor with position and speed
    //getters, const to prevent changing the actual variables
    int getRow() const;
    int getCol() const;
    int getHealth() const;
    int getSpeed() const;
    bool isAlive() const;
    //setters
    void setRow(int r);
    void setCol(int c);

    //functions
    void takeDamage(int dmg);  //dependent on tower upgrades
    void toMove(bool grid[20][20]); //receives grid to base movement logic on
};

#endif
