#include <iostream>
#include <string.h>
#include "Game.h"
#include "GamePlayer.h"

using namespace std;

#define MONSTER_SYMBOL 'M'
#define FIGHTER_SYMBOL 'F'
#define OBSTACLE_SYMBOL '+'
#define SPACE_SYMBOL ' '

void printComponent (GameComponent* grid) {
    if (grid->isMonster()) cout << MONSTER_SYMBOL;
    else if (grid->isFighter()) cout << FIGHTER_SYMBOL;
    else if (grid->isSpace()) cout << SPACE_SYMBOL;
    else if (grid->isObstacle()) cout << OBSTACLE_SYMBOL;
    else cout << "Encounter Unknown Type: " << grid->getType() << endl;
}


void printGameGrids (Game* game) {
    cout << "======= print the board ======" << endl;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            GameComponent* grid = game->gameGrids[j][i];

            printComponent(grid);

        }
        cout << "\n";

    }
}

void printPlayers (Game* game) {
    cout << " ======== print the players ======= " << endl;
    for (int i = 0; i < PLAYER_NUM; i++) {
        cout << "Player " << i << ": ";
        printComponent(game->players[i]);
        cout << ", HP: " << game->players[i]->getHp();
        cout << endl;
    }
}

int main () {
    Game* game = new Game();
    int playerIndex = 0;
    printGameGrids(game);
    printPlayers(game);
    int i = 0;
    while (1) {
        cout << " ------ ROUND " <<  i++ << "---------" << endl; 
        cout << "User Input: ";
        char c;
        cin >> c;
        cout << endl;

        if (c == 'W')
            game->players[playerIndex]->move(game, NORTH);
        else if (c == 'A') 
            game->players[playerIndex]->move(game, WEST);
        else if (c == 'S')
            game->players[playerIndex]->move(game, SOUTH);
        else if (c == 'D')
            game->players[playerIndex]->move(game, EAST);
        else if (c == 'J')
            game->players[playerIndex]->attack(game);
        printGameGrids(game);
        printPlayers(game);
    }

    delete game;
    return 0;
}