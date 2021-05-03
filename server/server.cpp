// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-server-application.
// Official Microsoft Documentation 

#undef UNICODE

#include "server.h"



int main(void)
{
    // Initialize game server that will take inputs from commServer
    Game* game = new Game(); // true is passed because we need to know if this is the server or not.

    // Initialize communication server that will interface with the clients
    CommunicationServer* commServer = new CommunicationServer();

    
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
        std::vector<std::pair<int,CLIENT_INPUT>> inputs;
        commServer->getClientInputs(inputs);
        

        // 2. Update the game state
        GameActions actions;
        // populate the player inputs with NO_MOVE
        for (auto i = 0; i < PLAYER_NUM; i++) actions.playersInputs[i] = NO_MOVE;
        // fill in the movement for corresponding player
        for (auto iter = inputs.begin(); iter < inputs.end(); iter++) {
            std::pair<int, CLIENT_INPUT> input = *iter;
            actions.playersInputs[input.first] = input.second; 
        }

        game->handleInputs(actions.playersInputs);
        game->updateGameEvents();
        if (game->started) {
            game->updateProjectiles(); // should this be a good place to update projectile?
            game->updateBeacon(); // used to determine players inside the beacon capture area
            game->checkEnd();
        }


        // 3. Send the latest GameState to client
        // commServer->sendGameState(game->getGameState());
        commServer->sendGameUpdates(game->updates);
        game->clearUpdates();

        // 4. Wait until tick ends
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = end - start;
        while(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() < TICK_TIME) {
            end = std::chrono::steady_clock::now();
            duration = end - start;
        }
    }
    
    delete game;
    return 0;
}