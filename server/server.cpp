// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-server-application.
// Official Microsoft Documentation 

#undef UNICODE

#include "server.h"



int main(void)
{

    // Initialize communication server that will interface with the clients
    CommunicationServer* commServer = new CommunicationServer();

    // Initialize game server that will take inputs from commServer
    Game* game = new Game(); 
    
    /**
     * Basic server architecture:
     *  1. Receive client input
     *  2. Update the game state
     *  3. Send the updated state to clients
     *  4. Wait until tick ends
     */
    while(1) {

        auto start = std::chrono::steady_clock::now();

        // 1. Receive client input
        std::vector<std::pair<int,GAME_INPUT>> inputs;
        commServer->getClientInputs(inputs);
        

        // 2. Update the game state
        GameActions actions;
        // populate the player inputs with NO_MOVE
        for (auto i = 0; i < PLAYER_NUM; i++) actions.playersInputs[i].input = NO_MOVE;
        // fill in the movement for corresponding player
        for (auto iter = inputs.begin(); iter < inputs.end(); iter++) {
            std::pair<int, GAME_INPUT> input = *iter;
            actions.playersInputs[input.first] = input.second; 
        }

        game->handleInputs(actions.playersInputs);
        game->updateGameEvents();

        bool hasGameEnded = false;
        if (game->started) {
            game->updateProjectiles(); // used to update the exsiting projectiles in the game
            game->updateBeacon(); // used to determine players inside the beacon capture area
            game->checkEvoLevel(); // used to determine monster evolution level
            game->updateSafeRegion();
            game->updateDeath();
            hasGameEnded = game->checkEnd(); // used to determine whether the game has ended
        }


        // 3. Send the latest GameState to client
        // commServer->sendGameState(game->getGameState());
        commServer->sendGameUpdates(game->updates);
        game->clearUpdates();

        // 4. Wait until tick ends
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = end - start;
        // printf("Milliseconds per tick: %d\n", std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
        while(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() < TICK_TIME) {
            end = std::chrono::steady_clock::now();
            duration = end - start;
        }

    }
    commServer->cleanup();
    delete commServer;
    delete game;
    return 0;
}