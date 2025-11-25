#ifndef AI_H
#define AI_H
#include <iostream>
#include "Tower.h"
#include <vector>
using namespace std;

//This file declares the AI class

class AI{
private:             //declare all private values for AI
    int currentWave;
    int enemiesSpawned;
    int enemyHealth; //this health is not correspondent to the enemy's own health property, it uses this variable to implement the
                    //health upgrades as a result of rounds where the threshold for healthboost is passed
    int fastEnemiesPerWave;

public:
    AI(int fastEnemies);
    //getters, const to not change the values
    int getCurrentWave() const;
    int getEnemiesSpawned() const;
    int getEnemyHealth() const;

    //functions
    void nextWave(int playerScore);    //to determine health for next wave
    bool shouldSpawnEnemy() const;     //if less than 10, spawn enemy, const to make sure there are no weird changes to this
    int chooseSpawnColumn(bool grid[20][20], const vector<Tower>& towers); //for adaptive spawning, takes the grid with its T/F states and a reference to the towers vector
    int determineEnemySpeed();  //to decide which enemies go fast of the 10
    void incrementSpawned();   //to add the quantity of spawned enemies
    bool allWavesComplete() const; //Used as endgame condition
};

#endif
