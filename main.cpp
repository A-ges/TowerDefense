#include "Game.h"
#include <iostream>
#include <limits>            // The snippet for input validation is adopted from: https://www.geeksforgeeks.org/cpp/how-to-validate-user-input-in-cpp/
//later I realized this could probably be implemented using templates, but since its functional and spread across the code, i stuck with this implementation
using namespace std;

int main(){
    cout << "========================================\n";
    cout << "       CASTLE DEFENDER GAME\n";
    cout << "========================================\n";
    cout << "Defend your castle from enemy waves!\n";
    cout << "Place towers to destroy enemies.\n";
    cout << "Don't let enemies reach the castle!\n";
    cout << "========================================\n\n";

    int Difficulty;
    cout << "Please set your difficulty (fast enemies per round) [int value between 0-10]: ";
    while(true){
        //do not stop checking until valid (both type == int and [0-10]
        //First the code checks if the type == int, look at top of file for reference
    if(!(cin >> Difficulty)){
        cout << "Invalid input. Please enter an integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //cleans all characters from buffer
        continue;
        }

        //Then it checks if it falls in 0-10 with the or operator
    if(Difficulty < 0 || Difficulty > 10){
        cout << "Out of range. Please enter a number between 0 and 10: ";
        continue;
        }
        break;
    }

    Game game(Difficulty);      //create the game object, which takes difficulty in its constructor

    game.placeTowers(); //before the loop starts, let the player pick where they place the towers

    cout << "\nStarting the game, good luck and have fun\n";
    cout << "\nLegend: C = Castle, T = Tower, E = Enemy, . = Empty\n";

    game.gameLoop(); //start the game loop

    cout << "\nThank you for playing Castle Defender!\n"; //final end message to confirm the gameloop is over

    return 0;
}
