// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-server-application.
// Official Microsoft Documentation 

#undef UNICODE

#include "server.h"



int main(void)
{
    // Initialize game server that will take inputs from commServer
    Game* game = new Game();

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
        for (auto i = 0; i < MAX_PLAYERS; i++) actions.playersInputs[i] = NO_MOVE;
        // fill in the movement for corresponding player
        for (auto iter = inputs.begin(); iter < inputs.end(); iter++) {
            std::pair<int, CLIENT_INPUT> input = *iter;
            actions.playersInputs[input.first] = input.second; 
        }

        game->handleInputs(actions.playersInputs);


        // 3. Send the latest GameState to client
        commServer->sendGameState(game->getGameState());

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