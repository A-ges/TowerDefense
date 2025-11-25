#ifndef CASTLE_H
#define CASTLE_H
#include <iostream>
using namespace std;

//This file declares the castle class

class Castle{
private:  //create the private variables needed for the castle class
    int row;
    int col;
    int health;

public:
    Castle();  //empty constructor as there are no parameters required, all are set in cpp file
    int getRow() const; //getters, I use constants to make sure the values are not manipulated through these calls
    int getCol() const;
    int getHealth() const;

    void takeDamage(int dmg); //functions for the castle class, this one subtracts damage from the health variable
    bool isDestroyed() const; //T/F state to signify whether the castle is still standing
};

#endif
