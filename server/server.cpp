// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-server-application.
// Official Microsoft Documentation 

#undef UNICODE

#include "server.h"


int main(void)
{
    // Initialize game server that will take inputs from commServer
    GameServer* gameServer = new GameServer();

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
        std::vector<char> gameState = gameServer->processInputs(inputs);


        // 3. Send the updated state to client
        commServer->sendGameState(gameState);

        // 4. Wait until tick ends
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = end - start;
        while(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() < TICK_TIME) {
            end = std::chrono::steady_clock::now();
            duration = end - start;
        }
    }
    return 0;
}