#ifndef GAME_H
#define GAME_H
#include "Tower.h"
#include "Enemy.h"
#include "Castle.h"
#include "AI.h"
#include <iostream>
#include <vector>
using namespace std;
//This file declares the game class

class Game{
private:  //declare all private values for tower
    bool grid[20][20];  //use booleans to set to row/col to true (occupied) or false
    Castle castle;
    std::vector<Tower> towers;    //use vector because player may choose less than five
    std::vector<Enemy> enemies;   //again a vector to only measure the active enemies
    AI ai;                        //use my AI object for spawning and difficulty
    int playerScore;              //values used for stats
    int enemiesDestroyed;
    int turnCount;

public:
    Game(int fastEnemiesPerWave); //constructor to use when creating the game from the main file

    void displayGrid(); //to cout the gridstate
    void placeTowers(); //initiate the tower placement phase
    void gameLoop();    //main loop, runs until win or loss
    void processTurn(); //increase turn count, then spawn enemies, move enemies, attack tower, check castle damage
    void spawnEnemy();  //spawn enemy at top row if wave not complete
    void moveEnemies(); //if living, move towards castle
    void towersAttack(); //attack enemies in range of towers
    void checkEnemiesAtCastle(); //check if alive enemies in row 19, then let them apply damage
    void upgradePhase(); //start tower upgrade phase after wave
    void displayStats(); //called to show current stats
    void displayEndScreen(bool playerWon); //final debrief when game is done, parameter determines which of two messages (T: Win, F: Loss)
    bool isGameOver();   //true if castle destroyed OR all waves complete, false otherwise
};

#endif
