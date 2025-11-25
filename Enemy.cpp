#include "Enemy.h"
#include <iostream>
using namespace std;
//This file implements the enemy class



Enemy::Enemy(int c, int h, int s){ //full constructor
    row = 0;
    col = c;
    health = h;
    speed = s;
    alive = true;
}
 //getters
int Enemy::getRow() const{
    return row;
}

int Enemy::getCol() const{
    return col;
}

int Enemy::getHealth() const{
    return health;
}

int Enemy::getSpeed() const{
    return speed;
}

bool Enemy::isAlive() const{
    return alive;
}

//setters
void Enemy::setRow(int r){
    row = r;
}

void Enemy::setCol(int c){
    col = c;
}

//functions
void Enemy::takeDamage(int dmg){ //dmg is dependent on towers upgrade status
    health -= dmg;
    if(health <= 0){
        alive = false;
    }
}

//core movement towards castle logic for enemies
void Enemy::toMove(bool grid[20][20]){
    for(int s = 0; s < speed; s++){ //iterates speed times (max 2)
        if(row >= 19){ //enemy is at castle row, no moving
            break;
        }
        grid[row][col] = false;  //this sets the current position to empty to allow other enemies to enter that space, before enemy departs this space
        if(row + 1 < 20 && !grid[row + 1][col]){   //if next row is empty and within the bounds, row++
            row++;
        }
        else if(row + 1 < 20 && col - 1 >= 0 && !grid[row + 1][col - 1]){ //check if left diagonal is available (if tower is blocking)
            row++;
            col--;
        }
        else if(row + 1 < 20 && col + 1 < 20 && !grid[row + 1][col + 1]){ //check if right diagonal is available
            row++;
            col++;
        }
        grid[row][col] = true;  //New position is occupied now
    }
}
