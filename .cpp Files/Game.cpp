#include "Game.h"
#include <iostream>
#include <limits>     //Used for input validation
#include <cstdlib>    //used for random numbers
#include <ctime> //used to initialize the random numbers
using namespace std;
//This file implements the Game class

Game::Game(int fastEnemiesPerWave): ai(fastEnemiesPerWave){ //immediatly create AI once at the start with the game constructor trough member initialization
                                                            //it is created with the correct parameters exactly once as it makes a lot of appearances across
                                                            //this file
    for(int i = 0; i < 20; i++){            //setup the initial empty grid with a nested loop, setting all r/c spots to false (empty)

        for(int j = 0; j < 20; j++){
            grid[i][j] = false;

        }
        }

    castle = Castle();      //Castle is set at middle bottom automatically because of its constructor
    grid[19][9] = true;     //Set castle coordinate to true, because its filled

    playerScore = 0;
    enemiesDestroyed = 0;
    turnCount = 0;
    srand(time(0));   //Initialize the random numbers, adopted from: https://www.w3schools.com/cpp/cpp_howto_random_number.asp, using <cstdlib>
}

void Game::displayGrid() {
    //first add the basic numbers at the top for placement reference
    cout << "   "; //add space for good lining up
    for(int c = 0; c < 20; c++){
        if (c < 10) {  //on single digit numbers i place a space inbetween for readablity
            cout << " " << c;
        }else{
            cout << c; //Double digits take up too much space for spaces, but its still fairly readable so I decided to stick with it
        }
    }
    cout << "\n";
    for(int i = 0; i < 20; i++){
        //add row numbers
        if(i < 10){
            cout << " " << i << " ";
        }else{
            cout << i << " "; //same logic as columns
        }
        for(int j = 0; j < 20; j++){

            //if grid cell is false (unoccupied), print dot immediately
            if(!grid[i][j]){
                cout << " .";
                continue; //next iteration
            }
            //if grid[i][j] is true, now check objects in order: castle, tower, enemy
            if(i == castle.getRow() && j == castle.getCol()){
                cout << " C";
                continue;
            }
            for(size_t t = 0; t < towers.size(); t++){    //use size_t (unsigned int, never negative) because .size returns that and causes feedback messages if just int
                if(towers[t].getRow() == i && towers[t].getCol() == j){
                    cout << " T";
                    continue;
                }
            }
            for(size_t e = 0; e < enemies.size(); e++){
                if(enemies[e].isAlive() && enemies[e].getRow() == i && enemies[e].getCol() == j){
                    cout << " E";
                    continue;
                }
             }
        }

        cout << "\n";
    }
}


void Game::placeTowers(){    //First procedure of the game, explaining rules in a fun formatted way
    cout << "\n=== TOWER PLACEMENT PHASE ===\n";
    cout << "You can place up to 5 towers.\n";
    cout << "Towers cannot be placed in top 2 rows or on castle.\n";
    cout << "Enter row and column, or (Row: -1, Column: -1) to finish placement if you want <5 towers.\n";

    for(int i = 0; i < 5; i++){    //iterate 5 times for 5 towers, but have a stop option if -1 -1
        displayGrid();             //display each time
        cout << "\nTower " << (i + 1) << " of 5:\n";   //Simple message to let user know how many towers left

        int row;
        int col;

        //Get valid row input trough same mechanisms, adoped from https://www.geeksforgeeks.org/cpp/how-to-validate-user-input-in-cpp/
        cout << "Enter Row: ";
        while(true){
            if(!(cin >> row)){
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //cleans buffer, this method is used troughout this file
                continue;
            }
            break;
        }

        //get valid column input, same mechanism as in main file, source: https://www.geeksforgeeks.org/cpp/how-to-validate-user-input-in-cpp/
        cout << "Enter Column: ";
        while(true){
            if(!(cin >> col)){
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        if(row == -1 || col == -1){  //stop condition if <5 towers desired by player
            break;
        }
        if(row < 2 || row >= 20 || col < 0 || col >= 20){    //handle inputs outside of range or too close to top row
            cout << "Invalid position! Tower must be in rows 2-19.\n";
            i--; //iterate again for the same tower index
            continue;
        }
        if(grid[row][col]){ //returns true if occupied (all spots have a t/f value for occupance)
            cout << "Position occupied! Choose another spot.\n";
            i--;
            continue;
        }
        towers.push_back(Tower(row, col)); //valid input gets added to the vector of all towers
        grid[row][col] = true;             //set occupance to true
        cout << "Tower placed at (" << row << ", " << col << ")!\n";
    }
    cout << "\n" << towers.size() << " towers placed. Starting game...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');    //this line clears the input for the turn progression '.' input, otherwise it displays an unnecessary error
}

void Game::processTurn(){
    turnCount++;  //Increase turn counter

    //order of the turn:
    spawnEnemy();           //1. AI spawns new enemy at top
    moveEnemies();          //2. All enemies move toward castle
    towersAttack();         //3. All towers attack enemies in range
    checkEnemiesAtCastle(); //4. Check if any enemies reached castle

    //display current state and stats
    displayGrid();
    displayStats();
    string input;
    while (true) {
        cout << "\nType '.' to continue to next turn: ";
        getline(cin, input);

        if(input == "."){
            break;
        }else{
            cout << "Invalid input. To continue round enter '.'";
    }
}

}

void Game::spawnEnemy(){     //fully use the AI component to determine semi-tactical-spawning (explained in report)
    if(ai.shouldSpawnEnemy() && !ai.allWavesComplete()){   //enemies spawn in not yet 10 per wave and waves not complete
        int col = ai.chooseSpawnColumn(grid, towers);        //find column to spawn, dependent on grid state (towers)

        if(col != -1){      //check if AI spawns in valid column, during testing they sometimes disappeared, this condition fixed that
            int speed = ai.determineEnemySpeed(); //determine if enemy should be fast (depends on the difficulty)
            Enemy newEnemy(col, ai.getEnemyHealth(), speed); //initialize a new enemy
            enemies.push_back(newEnemy);  //add it to the vector with enemies
            grid[0][col] = true;  //occupy spawnpoint
            ai.incrementSpawned(); //increment spawned enemies by one

            cout << "\nEnemy spawned at column " << col; //notify if an enemy spawned
            if(speed == 2){
                cout << " (FAST!)";  //add whether that was a fast enemy
            }
            cout << "\n";
        }
    }
}

void Game::moveEnemies() {
    for(size_t i = 0; i < enemies.size(); i++){        //use size_t again because of the .size comparison, iterate over all enemies
        if(enemies[i].isAlive()){                      //move if alive
            enemies[i].toMove(grid);
        }
    }
}

void Game::towersAttack(){
    for(size_t t = 0; t < towers.size(); t++){        //find all towers and iterate
        for(size_t e = 0; e < enemies.size(); e++){  //iterate over all enemies
            if(enemies[e].isAlive() && towers[t].isInRange(enemies[e].getRow(), enemies[e].getCol())){ //if enemy e alive and in range tower t, trigger
                enemies[e].takeDamage(towers[t].getDamage()); //Enemies take damage, which is determined by towers damage getter method.
                                                              // I use this because of the possible upgrades applied
                if(!enemies[e].isAlive()){
                    grid[enemies[e].getRow()][enemies[e].getCol()] = false; //empty spot if enemy is dead
                    playerScore += 10;
                    enemiesDestroyed++;
                    cout << "Enemy destroyed by tower at ("
                         << towers[t].getRow() << ", "
                         << towers[t].getCol() << ")!\n";
                }
                break; //breaking here (checking just one enemy) is a design choice.
                       //I think tower placement becomes more tactical and upgrading is more rewarding this way
            }
        }
    }
    for(int i = enemies.size() - 1; i >= 0; i--){ //I start from the back to make sure i dont skip enemies after i remove one from the vector
        if(!enemies[i].isAlive()){
            enemies.erase(enemies.begin() + i);   //removes a killed enemy from the vector to keep it updated
        }
    }
}

void Game::checkEnemiesAtCastle(){
    for(int i = enemies.size() - 1; i >= 0; i--){ //Again, loop through all enemies backwards because some elements may be removed
        if(enemies[i].isAlive() && enemies[i].getRow() >= 19){ //Alive in the final row
            castle.takeDamage(10);
            grid[enemies[i].getRow()][enemies[i].getCol()] = false; //Grid spot of enemy is emptied, as its done its damage
            enemies.erase(enemies.begin() + i);  //remove that enemy from the enemy vector
            cout << "Enemy reached castle! -10 health!\n";
        }
    }
}

void Game::upgradePhase() {
    cout << "\n=== TOWER UPGRADE PHASE ===\n";
    cout << "You can upgrade towers (Cost: 30 points per upgrade)\n";
    cout << "Your current score: " << playerScore << "\n";
    cout << "Upgrade increases tower damage and range by 1.\n";

    //keep allowing upgrades until player quits or runs out of points
    while(true){
        //check if player can afford upgrade, if no it breaks
        if(playerScore < 30){
            cout << "Not enough points to upgrade!\n";
            break;
        }
        displayGrid();

        cout << "\nYour towers:\n";
        for(size_t i = 0; i < towers.size(); i++){
            cout << i + 1 << ". Position (" << towers[i].getRow()
                 << ", " << towers[i].getCol() << ") - Damage: "
                 << towers[i].getDamage() << " and Range: " << towers[i].getRange() << "\n";  //Gives a nice overview of towers and their states
        }
        int row;
        int col;

        cout << "\nEnter row to upgrade (or -1 to skip): ";
        while(true){
            if(!(cin >> row)){
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');   //same principle, again used https://www.geeksforgeeks.org/cpp/how-to-validate-user-input-in-cpp/
                continue;
            }
            break;
        }

        // Check if user wants to quit upgrading
        if(row == -1){
            break;  // Exit upgrade phase
        }

        //Repeat past block for column validation
        cout << "Enter column to upgrade (or -1 to skip): ";
        while(true){
            if(!(cin >> col)){
                cout << "Invalid input. Please enter an integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //same principle as former block
                continue;
            }
            break;
        }

        //check if user wants to quit upgrading
        if(col == -1){
            break;  //exit condition triggered
        }

        //Now the procedure to find whether a tower is actually on the input coordinates
        bool found = false;  //initialize as false
        for(size_t i = 0; i < towers.size(); i++){   //check all towers
            if(towers[i].getRow() == row && towers[i].getCol() == col){ //if found, +dmg and -score
                towers[i].upgrade();
                playerScore -= 30;
                cout << "Tower upgraded! New damage: " << towers[i].getDamage() << ", new range: "<< towers[i].getRange() << "\n";
                cout << "Remaining score: " << playerScore << "\n";

                found = true;
                break;  //stop looking for towers
            }
        }

        //If no tower found at that position, let player know and loop again
        if(!found){
            cout << "No tower at that position!\n";
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //this line clears the input buffer for the turn progression '.' input, otherwise it displays an unnecessary error


}

void Game::displayStats(){ //a nicely formatted stat screen
    cout << "\n--- Stats ---\n";
    cout << "Wave: " << ai.getCurrentWave() << "/5\n";
    cout << "Turn: " << turnCount << "\n";
    cout << "Score: " << playerScore << "\n";
    cout << "Enemies Destroyed: " << enemiesDestroyed << "\n";
    cout << "Active Enemies: " << enemies.size() << "\n";
    cout << "Castle Health: " << castle.getHealth() << "/100\n";
}

void Game::displayEndScreen(bool playerWon){                //end screen, last sentence dependent on who won (bool)
    cout << "\n========================================\n";
    cout << "            GAME OVER\n";
    cout << "========================================\n";
    cout << "Player Score: " << playerScore << "\n";
    cout << "Enemies Destroyed: " << enemiesDestroyed << "\n";
    cout << "Castle Health: " << castle.getHealth() << "/100\n";
    if (playerWon){
        cout << "Winner: PLAYER! Congratulations!\n";
    }else{
        cout << "Winner: AI. Try another time perhaps?\n";
    }
    cout << "========================================\n";
}

bool Game::isGameOver(){    //handy to know if i have to keep the gameloop going
    if(castle.isDestroyed()){ //condition 1
        return true;
    }

    if(ai.allWavesComplete() && enemies.size() == 0){ //condition 2
        return true;
    }

    return false;
}

void Game::gameLoop() {       //main game loop
    while(!isGameOver()){     //condition is true if all waves completed or castle health = 0
        processTurn();

        if(ai.getEnemiesSpawned() >= 10 && enemies.size() == 0){      //check if wave complete
            if(!ai.allWavesComplete()){                               //check if all waves are not complete
                cout << "\n=== WAVE " << ai.getCurrentWave() << " COMPLETE ===\n";
                displayStats();
                ai.nextWave(playerScore); //strength of AIs in next wave is determined by score

                if(!ai.allWavesComplete() && !isGameOver()){
                    upgradePhase();
                    cout << "\n=== WAVE " << ai.getCurrentWave() << " STARTING ===\n";
            }
            }
        }
    }
    //in case game has ended, determine if win/loss
    bool playerWon = !castle.isDestroyed();
    displayEndScreen(playerWon); //display corresponding end message
}
