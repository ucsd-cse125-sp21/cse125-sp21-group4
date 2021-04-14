/** Communication Server
 * - Solely responsible for communicating data with the clients
 * - Pass information to the GameServer to handle physics, collisions, and objects
 */
#ifndef COMMUNICATION_SERVER_H
#define COMMUNICATION_SERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Includes the required libraries for WinSock2 Server
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <thread>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#include "../common/constants.h"
#include "../common/PlayerPosition.h"
#include "../common/GameState.h"
#include "../common/game/GameConstants.h"

struct PlayerInfo {
    int id;
    CLIENT_INPUT input;
    bool outputChanged;
    std::vector<char> inputs;
};

class CommunicationServer {
private:
    std::thread playerThreads[MAX_PLAYERS]; // All the player threads
    SOCKET clientSockets[MAX_PLAYERS]; // client socket fd for each player
    PlayerInfo playerInfos[MAX_PLAYERS]; // Player info (like their input, output, and id)

    static void cleanUpSocket(SOCKET* clientSocketPtr);
public:
    CommunicationServer(); // Constructs the server and threads required for 4 players

    void cleanup(); // Ends the game

    void getClientInputs(std::vector<std::pair<int,CLIENT_INPUT>> &clientInputs); // Obtains all the inputs from the clients for processing in the GameServer
    void sendGameState(GameState gameState); // Sends the new game state to each client

    static int handlePlayerThread(SOCKET* clientSocketPtr, PlayerInfo* playerInfo); // Thread for socket that will handle sending/receiving info


};

#endif