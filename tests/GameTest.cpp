#include <iostream>
#include <string.h>
#include "../common/game/Game.h"
#include "../common/game/GamePlayer.h"

using namespace std;

int main () {
    Game* game = new Game();
    int playerIndex = 0;
    game->printGameGrids();
    game->printPlayers();
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
        game->printGameGrids();
        game->printPlayers();
    }

    delete game;
    return 0;
}