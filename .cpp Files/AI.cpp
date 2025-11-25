#include "AI.h"
#include <iostream>
#include <cstdlib> //random numbers
#include <cmath>  //abs function
using namespace std;

//This file implements the AI class
AI::AI(int fastEnemies){  //full constructor with starting conditions
    currentWave = 1;
    enemiesSpawned = 0;
    enemyHealth = 3;
    fastEnemiesPerWave = fastEnemies;
}
//getters
int AI::getCurrentWave() const{
    return currentWave;
}

int AI::getEnemiesSpawned() const{
    return enemiesSpawned;
}

int AI::getEnemyHealth() const{
    return enemyHealth;
}

//functions
void AI::nextWave(int playerScore){    //enemy health updates for new wave for adaptive difficulty system
    int waveThreshold = currentWave * 80; //Score for required health upgrade
    if(playerScore >= waveThreshold){
        enemyHealth++;            //If satisfied do health upgrade
    }
    currentWave++;
    enemiesSpawned = 0;
}

bool AI::shouldSpawnEnemy() const{
    return enemiesSpawned < 10; //true if less than 10 spawned in wave
}

//Semi-strategic spawning, 30% chance of enemies choosing most weak spot (otherwise they have a straight line)
int AI::chooseSpawnColumn(bool grid[20][20], const vector<Tower>& towers){  //returns col number, pass by tower vector reference
    bool availableColumns[20]; //create an array for all possible positions, used if no strategical spawning and just have to pick at random
    for(int i = 0; i < 20; i++){
        availableColumns[i] = !grid[0][i];  //add values to that array to which 20 spots are not occupied
    }

    //30% chance to use strategic spawning
    bool useStrategy = (rand() % 100) < 30;  //adopted from https://www.w3schools.com/cpp/cpp_howto_random_number.asp

    if(useStrategy && towers.size() > 0){  //in case of strat and if the enemy has at least one tower to work with
        vector<int> safeColumns;           //this vector will hold all strategized spots (out of reach of any tower)
        for(int c = 0; c < 20; c++){       //iterate all spots
            bool isSafe = true;            //check whether its safe, init as true
            for(size_t i = 0; i < towers.size(); i++){    //first get all towers //get how they are positioned relative to the column under investigation //If less than safe distance, disregard the space
                int colDiff = abs(towers[i].getCol() - c); //get horizontal difference
                if (colDiff <= towers[i].getRange()) {  //If range within this horizontal difference, its unsafe
                isSafe = false;
                break;
                }
            }
            if(isSafe){
                safeColumns.push_back(c);          //it is a strategic spot, add to the vector
             }
        }
        if(safeColumns.size() > 0){            //to pick one of the available strategical spots, just get a random one for enemy spread
            int randomIndex = rand() % safeColumns.size();
            return safeColumns[randomIndex];
        }
    }
    //default: spawn randomly at any available column is simple procedure, pick a random one from the list of available spots
    vector<int> anyColumns;
    for(int c = 0; c < 20; c++){
        if (availableColumns[c]){
            anyColumns.push_back(c); //vector of indexes available spots
        }
    }
    if(anyColumns.size() > 0){
        int randomIndex = rand() % anyColumns.size();  //randomly pick one of the spots
        return anyColumns[randomIndex];
    }
    return -1;  // it gives an unfatal error if i dont put an "all fail" condition, -1 is irrelevant because there will always be a spot with current settings
}

int AI::determineEnemySpeed(){ //first i put some shortcuts for the simple situations (all or nothing), then for sequential determining
    if(fastEnemiesPerWave == 0){ //If no fast enemies configured, all enemies are normal speed, prevents a zero division error
        return 1;  // = normal speed
    }
    if(fastEnemiesPerWave == 10){ //if all enemies should be fast
        return 2;  //all enemies speed will set to 2
    }
    int fastEnemiesSpawned = 0; //keep track of how many spawned

    //I create a good split, if there are 2 fast enemies, enemy 0 and 5 would be fast
    //reconstruct how many fast enemies have appeared so far.
    for(int i = 0; i < enemiesSpawned; i++){ //Check if this enemy slot should be fast
        if(i % (10 / fastEnemiesPerWave) == 0 && fastEnemiesSpawned < fastEnemiesPerWave){ //i falls on good even spot and we havent reached total num of enemies
            fastEnemiesSpawned++;
        }
    }

    //Decide if the next enemy at index enemiesspawned should be fast.
    //Check if the upcoming index is a good split spot
    //and if we still have fast enemies left to assign.
    if(fastEnemiesSpawned < fastEnemiesPerWave && (enemiesSpawned % (10 / fastEnemiesPerWave) == 0)){
        return 2;  //This enemy is fast
    }

    return 1;  //This enemy is normal speed
}

void AI::incrementSpawned() { //to increment amount of spawns
    enemiesSpawned++;
}

bool AI::allWavesComplete() const { //used as endgame condition
    return currentWave > 5;
}
